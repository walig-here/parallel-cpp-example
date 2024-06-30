#include <iostream>

#include "EmployeeThread.h"
#include "CofeeDispenser.h"
#include "UserInterfaceThread.h"
#include "EmployeeUI.h"
#include "RandomNumberGenerator.h"

const int EmployeeThread::STANDS_AT_WORKSTATION = -1;

void EmployeeThread::main()
{
    this->current_state = new EmployeeWorking(this);
    this->next_state = this->current_state;
    while (true)
    {
        this->current_state->performActivity();
        if (this->current_state != this->next_state)
        {
            delete this->current_state;
            this->current_state = this->next_state;
        }
    }
}

void EmployeeThread::setNextState(EmployeeState *next_state)
{
    if (this->next_state != nullptr && this->current_state != this->current_state)
        delete this->next_state;
    if (next_state != nullptr)
        this->next_state = next_state;
    else
        this->next_state = this->current_state;
}

EmployeeThread::EmployeeThread(const int& initial_energy):
    Thread("Emp"),
    current_dispenser_index(EmployeeThread::STANDS_AT_WORKSTATION),
    energy(initial_energy),
    current_state(nullptr),
    next_state(nullptr)
{
    RandomNumberGenerator rng;
    burn_1_energy_time_miliseconds = rng.getInt(500, 5000);
}

EmployeeThread::~EmployeeThread()
{
    delete this->current_state;
    if (this->next_state != nullptr)
        delete this->next_state;
}

void EmployeeThread::drinkCoffee(int amount_of_coffee)
{
    this->energy += amount_of_coffee;
    this->nofityAll(EmployeeUI::SET_ENERGY + " " + std::to_string(this->energy));
}

EmployeeState::EmployeeState(EmployeeThread *owner)
{
    this->owner = owner;
}

EmployeePickingDispenser::EmployeePickingDispenser(EmployeeThread *owner): 
    EmployeeState(owner) {}

void EmployeePickingDispenser::performActivity()
{
    RandomNumberGenerator rng;
    this->owner->current_dispenser_index = rng.getInt(0, CoffeeDispenser::getNumberOfDispensers()-1);
    
    this->owner->setNextState(new EmployeeBuyingCoffee(this->owner));
    this->owner->nofityAll(EmployeeUI::SET_INDEX + ' ' + std::to_string(this->owner->getCurrentDispenserIndex()));
}

void EmployeeBuyingCoffee::performActivity()
{
    CoffeeDispenser& dispenser = CoffeeDispenser::getDispenser(this->owner->current_dispenser_index);
    dispenser.buyCoffee(*(this->owner));
    this->owner->setNextState(new EmployeeWorking(this->owner));
    this->owner->nofityAll(EmployeeUI::SET_ENERGY + ' ' + std::to_string(this->owner->getEnergy()));
}

EmployeeWorking::EmployeeWorking(EmployeeThread *owner) : EmployeeState(owner){}

void EmployeeWorking::performActivity()
{
    this->owner->current_dispenser_index = EmployeeThread::STANDS_AT_WORKSTATION;
    this->owner->nofityAll(EmployeeUI::SET_INDEX + ' ' + std::to_string(this->owner->getCurrentDispenserIndex()));


    std::this_thread::sleep_for(std::chrono::milliseconds(this->owner->burn_1_energy_time_miliseconds));
    if (this->owner->getEnergy() <= 0)
    {
        EmployeeState* next_state = new EmployeePickingDispenser(this->owner);
        this->owner->setNextState(next_state);
    }
    else
    {
        this->owner->energy--;
        this->owner->nofityAll(EmployeeUI::SET_ENERGY + ' ' + std::to_string(this->owner->getEnergy()));
    }
    
}
