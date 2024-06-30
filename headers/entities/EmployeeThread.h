#ifndef EMPLOYEE_THREAD
#define EMPLOYEE_THREAD

#include "UserInterfaceThread.h"
#include "Subject.h"

class EmployeeState;

class EmployeeThread : public Thread, public Subject
{
public:
    static const int STANDS_AT_WORKSTATION;

    EmployeeThread(const int& initial_energy=10);
    ~EmployeeThread();
    int getCurrentDispenserIndex() {return this->current_dispenser_index;}
    int getEnergy() {return this->energy;}
    void drinkCoffee(int amoun_of_coffee);

protected:

    friend class EmployeeWorking;
    friend class EmployeePickingDispenser;
    friend class EmployeeBuyingCoffee;

    int current_dispenser_index;    
    int energy; 
    int burn_1_energy_time_miliseconds;

    virtual void main();
    void setNextState(EmployeeState* next_state);

private:
                  
    EmployeeState* current_state;
    EmployeeState* next_state;
};

class EmployeeState
{
protected:

    friend class EmployeeThread;

    EmployeeThread* owner;

    EmployeeState(EmployeeThread* owner);
    virtual ~EmployeeState() {};
    virtual void performActivity() = 0;
};

class EmployeeWorking : public EmployeeState
{
protected:

    friend class EmployeeThread;
    friend class EmployeeBuyingCoffee;

    EmployeeWorking(EmployeeThread* owner);
    virtual ~EmployeeWorking() {};
    virtual void performActivity();
};

class EmployeePickingDispenser : public EmployeeState
{
protected:

    friend class EmployeeThread;
    friend class EmployeeWorking;

    EmployeePickingDispenser(EmployeeThread* owner);
    virtual ~EmployeePickingDispenser() {};
    virtual void performActivity();
};

class EmployeeBuyingCoffee : public EmployeeState
{
protected:

    friend class EmployeeThread;
    friend class EmployeePickingDispenser;

    EmployeeBuyingCoffee(EmployeeThread* owner): EmployeeState(owner) {};
    virtual ~EmployeeBuyingCoffee() {};
    virtual void performActivity();
};

#endif
