#include <iostream>
#include <algorithm>

#include "CofeeDispenser.h"
#include "EmployeeThread.h"
#include "DispenserUI.h"
#include "RandomNumberGenerator.h"


std::vector<CoffeeDispenser*> CoffeeDispenser::all_dispensers; 
std::condition_variable CoffeeDispenser::does_any_dispenser_need_maintainer;
std::mutex CoffeeDispenser::dispensers_list_mutex;

const int CoffeeDispenser::COFFEE_RATION_FOR_ONE_EMPLOYEE = 5;
const int CoffeeDispenser::DISPENSING_COFFEE_TIME_SECONDS = 1; 
const int CoffeeDispenser::INSPECT_TIME_SECONDS = 1;
const int CoffeeDispenser::MAX_CAPACITY = 100;
const int CoffeeDispenser::REFILL_COFFEE_LEVEL = CoffeeDispenser::MAX_CAPACITY / 4;

CoffeeDispenser &CoffeeDispenser::getDispenser(int index)
{
    std::lock_guard lock(CoffeeDispenser::dispensers_list_mutex);
    return *CoffeeDispenser::all_dispensers[index];
}

int CoffeeDispenser::getNumberOfDispensers()
{
    std::lock_guard lock(CoffeeDispenser::dispensers_list_mutex);
    return CoffeeDispenser::all_dispensers.size();
}

int CoffeeDispenser::waitUntilAnyDispenserNeedsMaintainer(const MaintainerThread &maintainer)
{
    int dispenser_needing_maintainer_index = -1;
    std::unique_lock lock(CoffeeDispenser::dispensers_list_mutex);
    CoffeeDispenser::does_any_dispenser_need_maintainer.wait(
        lock, 
        [&dispenser_needing_maintainer_index]() mutable
        {
            for (int i = 0; i < CoffeeDispenser::all_dispensers.size(); i++)
            {
                if (!CoffeeDispenser::all_dispensers[i]->doNeedMainainer())
                    continue;
                dispenser_needing_maintainer_index = i;
                return true;
            }
            return false;
        }
    );
    return dispenser_needing_maintainer_index;
}

void CoffeeDispenser::notifyDispenserNeedsMaintainer()
{
    CoffeeDispenser::does_any_dispenser_need_maintainer.notify_all();
}

CoffeeDispenser::CoffeeDispenser(const int &initial_cofee) : 
    coffee((initial_cofee > CoffeeDispenser::MAX_CAPACITY ? CoffeeDispenser::MAX_CAPACITY : initial_cofee)),
    is_broken(false),
    is_assigned_to_maintainer(false)
{
    RandomNumberGenerator rng;
    breaking_probability_percent = rng.getInt(5, 20);

    std::lock_guard(CoffeeDispenser::dispensers_list_mutex);
    CoffeeDispenser::all_dispensers.push_back(this);
    CoffeeDispenser::dispensers_list_mutex.unlock();
    CoffeeDispenser::notifyDispenserNeedsMaintainer();
}

CoffeeDispenser::~CoffeeDispenser()
{
    std::lock_guard(CoffeeDispenser::dispensers_list_mutex);
    auto dispenser = std::remove(
        CoffeeDispenser::all_dispensers.begin(), 
        CoffeeDispenser::all_dispensers.end(),
        this
    );
    CoffeeDispenser::all_dispensers.erase(dispenser);
}

void CoffeeDispenser::buyCoffee(EmployeeThread &buyer)
{
    this->nofityAll(DispenserUI::QUEUE_THREAD + " " + buyer.getId());
    std::unique_lock lock(this->access_dispenser_mutex);
    this->queue_to_dispenser.push(buyer.getId());

    this->is_dispenser_functional.wait(
        lock, 
        [this, &buyer] { 
            if ((this->coffee < CoffeeDispenser::COFFEE_RATION_FOR_ONE_EMPLOYEE || this->is_broken) && 
                !this->is_assigned_to_maintainer)
                CoffeeDispenser::notifyDispenserNeedsMaintainer();

            return this->coffee >= CoffeeDispenser::COFFEE_RATION_FOR_ONE_EMPLOYEE &&
                   !this->is_broken &&
                   this->queue_to_dispenser.front() == buyer.getId(); 
        }
    );

    std::this_thread::sleep_for(std::chrono::seconds(CoffeeDispenser::DISPENSING_COFFEE_TIME_SECONDS));
    buyer.drinkCoffee(CoffeeDispenser::COFFEE_RATION_FOR_ONE_EMPLOYEE);
    this->coffee -= CoffeeDispenser::COFFEE_RATION_FOR_ONE_EMPLOYEE;
    this->nofityAll(DispenserUI::SET_COFFEE + " " + std::to_string(this->getAmountOfCoffee()));

    this->simulate_breaking();

    this->queue_to_dispenser.pop();
    this->nofityAll(DispenserUI::DEQUEUE_THREAD + " " + buyer.getId());
    this->is_dispenser_functional.notify_all();
}

void CoffeeDispenser::maintain(MaintainerThread &maintainer)
{
    this->nofityAll(DispenserUI::QUEUE_THREAD + " " + maintainer.getId());
    this->access_dispenser_mutex.lock();

    std::this_thread::sleep_for(std::chrono::seconds(CoffeeDispenser::INSPECT_TIME_SECONDS));
    
    // is dispenser broken?
    if (this->is_broken)
    {
        this->nofityAll(DispenserUI::MOVE_TO_QUEUE_HEAD + " " + maintainer.getId());
        std::this_thread::sleep_for(std::chrono::milliseconds(this->repair_time_miliseconds));
        this->is_broken = false;
        this->nofityAll(DispenserUI::SET_BROKEN + " 0");
        this->is_assigned_to_maintainer = false;
        this->is_dispenser_functional.notify_all();
    }
    // is dispenser running out of coffee?
    if (this->coffee <= CoffeeDispenser::REFILL_COFFEE_LEVEL)
    {
        this->nofityAll(DispenserUI::MOVE_TO_QUEUE_HEAD + " " + maintainer.getId());
        while(int coffee = maintainer.popCoffee())
        {
            this->coffee += coffee;
            this->nofityAll(DispenserUI::SET_COFFEE + " " + std::to_string(this->coffee));
            if (this->coffee == CoffeeDispenser::MAX_CAPACITY)
                break;
        }
        this->is_assigned_to_maintainer = false;
        this->is_dispenser_functional.notify_all();
    }

    this->access_dispenser_mutex.unlock();
    this->nofityAll(DispenserUI::DEQUEUE_THREAD + " " + maintainer.getId());
}

bool CoffeeDispenser::doNeedMainainer()
{
    if (this->access_dispenser_mutex.try_lock())
    {
        if ((this->is_broken || this->coffee <= CoffeeDispenser::REFILL_COFFEE_LEVEL) &&
            !this->is_assigned_to_maintainer)
        {
            this->is_assigned_to_maintainer = true;
            this->access_dispenser_mutex.unlock();
            return true;
        }
        this->access_dispenser_mutex.unlock();
        return false;
    }
    return false;
}

void CoffeeDispenser::simulate_breaking()
{
    RandomNumberGenerator rng;
    if (rng.getInt(0, 100) > breaking_probability_percent)
        return;
    is_broken = true;
    this->repair_time_miliseconds = rng.getInt(800, 2500);
    this->nofityAll(DispenserUI::SET_BROKEN + " 1");
}
