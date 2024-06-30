#ifndef MAINTAINER_THREAD_H
#define MAINTAINER_THREAD_H

#include "Thread.h"
#include "Subject.h"

class MaintainerState;


class MaintainerThread : public Thread, public Subject
{
public:

    MaintainerThread();
    ~MaintainerThread();
    int popCoffee();
    int getCurrentDispenserIndex() { return this->current_dispenser_index; }
    bool pushCoffee();
    int getMaxCoffee() {return this->coffee_capacity;}

protected:
    friend class MainainerInStorage;
    friend class MaintainerMaintainsDispensers;

    static const int MAINTAINER_IN_STORAGE;
    static const int MAINTAINER_NEAR_SHELF;

    int currently_held_coffee;
    int current_dispenser_index;

    virtual void main();
    void setNextState(MaintainerState* next_state);

private:

    MaintainerState* current_state;
    MaintainerState* next_state;
    int move_coffee_time_miliseconds;
    int coffee_capacity;
};


class MaintainerState
{
protected:
    friend class MaintainerThread;

    MaintainerState(MaintainerThread* owner);
    ~MaintainerState();
    virtual void performActivity() = 0;

    MaintainerThread* owner;
};


class MaintainerMaintainsDispensers: public MaintainerState
{
protected:
    friend class MaintainerThread;
    friend class MainainerInStorage;

    MaintainerMaintainsDispensers(MaintainerThread* owner);
    ~MaintainerMaintainsDispensers();
    virtual void performActivity();    
};


class MainainerInStorage : public MaintainerState
{
protected:
    friend class MaintainerThread;
    friend class MaintainerMaintainsDispensers;

    MainainerInStorage(MaintainerThread* owner);
    ~MainainerInStorage();
    virtual void performActivity();

};

#endif
