#include <algorithm>
#include <sstream>
#include <iomanip>
#include <ranges>

#include "DispenserUI.h"
#include "EmployeeUI.h"
#include "MaintainerUI.h"


const std::string DispenserUI::SET_COFFEE           = "set-coffee";
const std::string DispenserUI::QUEUE_THREAD         = "queqe-thread";
const std::string DispenserUI::DEQUEUE_THREAD       = "dequeue-thread";
const std::string DispenserUI::MOVE_TO_QUEUE_HEAD   = "move-to-head";
const std::string DispenserUI::SET_BROKEN           = "set-broken";

DispenserUI::DispenserUI(const int& dispenser_index, const int& coffee): 
    Observer("dispenser"),
    dispenser_index(dispenser_index),
    coffee(coffee),
    is_broken(false) 
    {}

DispenserUI::~DispenserUI()
{
}

void DispenserUI::update(const std::string message)
{
    std::istringstream parser(message);
    std::string mnemonic;
    parser >> mnemonic;

    if (mnemonic == DispenserUI::SET_COFFEE)
    {
        parser >> this->coffee;
    }
    else if (mnemonic == DispenserUI::QUEUE_THREAD)
    {
        std::string thread_id;
        parser >> thread_id;

        queue_to_dispenser_mutex.lock();
        this->queue_to_dispenser.push_back(thread_id);
        queue_to_dispenser_mutex.unlock();
    }
    else if (mnemonic == DispenserUI::DEQUEUE_THREAD)
    {
        std::string thread_id;
        parser >> thread_id;

        queue_to_dispenser_mutex.lock();
        auto it = std::remove(
            this->queue_to_dispenser.begin(),
            this->queue_to_dispenser.end(),
            thread_id
        );
        if (it != this->queue_to_dispenser.end())
            this->queue_to_dispenser.erase(it);
        queue_to_dispenser_mutex.unlock();
    }
    else if (mnemonic == DispenserUI::MOVE_TO_QUEUE_HEAD)
    {
        std::string thread_id;
        parser >> thread_id;

        queue_to_dispenser_mutex.lock();
        auto it = std::remove(
            this->queue_to_dispenser.begin(),
            this->queue_to_dispenser.end(),
            thread_id
        );
        if (it != this->queue_to_dispenser.end())
        {
            this->queue_to_dispenser.erase(it);
            this->queue_to_dispenser.insert(this->queue_to_dispenser.begin(), thread_id);
        }
        queue_to_dispenser_mutex.unlock();
    }
    else if (mnemonic == DispenserUI::SET_BROKEN)
    {
        parser >> this->is_broken;
    }
}

std::string DispenserUI::toString(std::vector<EmployeeUI*>& employees, std::vector<MaintainerUI*>& maintainers)
{
    std::ostringstream result;
    result  << "#" << this->dispenser_index 
            << " [" << std::setw(3) << this->coffee << "/" << CoffeeDispenser::MAX_CAPACITY
            << "|" << (is_broken ? "-" : "+") << "]";
    
    queue_to_dispenser_mutex.lock();
    for (std::string thread_id : this->queue_to_dispenser)
    {
        auto employee = std::find_if(
            employees.begin(),
            employees.end(),
            [thread_id](auto employee_from_collection)
            {
                return employee_from_collection->getId() == thread_id;
            }
        );

        auto maintainer = std::find_if(
            maintainers.begin(),
            maintainers.end(),
            [thread_id](auto maintainer_from_collection)
            {
                return maintainer_from_collection->getId() == thread_id;
            }
        );

        if (employee != employees.end())
        {
            result << " " << (*employee)->toString();
            employees.erase(employee);
        }
        else if (maintainer != maintainers.end())
        {
            result << " " << (*maintainer)->toString();
            maintainers.erase(maintainer);
        }
    }
    queue_to_dispenser_mutex.unlock();
    
    return result.str();
}
