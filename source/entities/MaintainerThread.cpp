#include "MaintainerThread.h"
#include "MaintainerUI.h"
#include "CoffeeShelf.h"
#include "CofeeDispenser.h"
#include "RandomNumberGenerator.h"

const int MaintainerThread::MAINTAINER_IN_STORAGE = -1;
const int MaintainerThread::MAINTAINER_NEAR_SHELF = -2;

MaintainerThread::MaintainerThread():
    Thread("Mnt"),
    currently_held_coffee(0),
    current_state(nullptr),
    next_state(nullptr),
    current_dispenser_index(MaintainerThread::MAINTAINER_IN_STORAGE) 
{
    RandomNumberGenerator rng;
    this->move_coffee_time_miliseconds = rng.getInt(100, 500);
    this->coffee_capacity = rng.getInt(50, 150);
}

MaintainerThread::~MaintainerThread()
{
    delete this->current_state;
    if (this->next_state != nullptr)
        delete this->next_state;
}

int MaintainerThread::popCoffee()
{
    if (this->currently_held_coffee == 0)
        return 0;
    std::this_thread::sleep_for(std::chrono::milliseconds(this->move_coffee_time_miliseconds));
    this->currently_held_coffee--;
    this->nofityAll(MaintainerUI::SET_COFFEE + " " + std::to_string(this->currently_held_coffee));
    return 1;
}

bool MaintainerThread::pushCoffee()
{
    if (this->currently_held_coffee == this->coffee_capacity)
        return false;
    std::this_thread::sleep_for(std::chrono::milliseconds(this->move_coffee_time_miliseconds));
    this->currently_held_coffee += 1;
    this->nofityAll(MaintainerUI::SET_COFFEE + " " + std::to_string(this->currently_held_coffee));
    return true;
}

void MaintainerThread::main()
{
    this->current_state = new MainainerInStorage(this);
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

void MaintainerThread::setNextState(MaintainerState *next_state)
{
    if (this->next_state != nullptr && this->current_state != this->current_state)
        delete this->next_state;
    if (next_state != nullptr)
        this->next_state = next_state;
    else
        this->next_state = this->current_state;
}

MaintainerState::MaintainerState(MaintainerThread *owner)
{
    this->owner = owner;
}

MainainerInStorage::MainainerInStorage(MaintainerThread *owner):
    MaintainerState(owner) {}

void MainainerInStorage::performActivity()
{
    if (this->owner->currently_held_coffee == 0)
    {
        this->owner->current_dispenser_index = MaintainerThread::MAINTAINER_NEAR_SHELF;
        this->owner->nofityAll(MaintainerUI::SET_INDEX + " " + std::to_string(this->owner->current_dispenser_index));
        CoffeeShelf::getInstance()->takeCofee(*(this->owner));
    }
    this->owner->current_dispenser_index = MaintainerThread::MAINTAINER_IN_STORAGE;
    this->owner->setNextState(new MaintainerMaintainsDispensers(this->owner));
}

MaintainerMaintainsDispensers::MaintainerMaintainsDispensers(MaintainerThread *owner):
    MaintainerState(owner) {}

void MaintainerMaintainsDispensers::performActivity()
{   
    this->owner->current_dispenser_index = CoffeeDispenser::waitUntilAnyDispenserNeedsMaintainer(*(this->owner));
    this->owner->nofityAll(MaintainerUI::SET_INDEX + " " + std::to_string(this->owner->current_dispenser_index));
    CoffeeDispenser& dispenser = CoffeeDispenser::getDispenser(this->owner->current_dispenser_index);
    dispenser.maintain(*(this->owner));

    this->owner->setNextState(new MainainerInStorage(this->owner));
}

MainainerInStorage::~MainainerInStorage() {}
MaintainerState::~MaintainerState() {}
MaintainerMaintainsDispensers::~MaintainerMaintainsDispensers() {}
