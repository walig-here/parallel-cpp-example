#ifndef USER_INTERFACE_THREAD_H
#define USER_INTERFACE_THREAD_H

#include <vector>

#include "Observer.h"
#include "Thread.h"
#include "CofeeDispenser.h"
#include "EmployeeThread.h"
#include "CoffeeShelf.h"

class DispenserUI;
class EmployeeUI;
class MaintainerUI;
class ShelfUI;
typedef const std::string command;

class UserInterfaceThread : public Thread
{
public:

    UserInterfaceThread(CoffeeShelf& shelf);
    ~UserInterfaceThread();
    void addDispenser(CoffeeDispenser& dispenser);
    void addEmployee(EmployeeThread& employee);  
    void addMaintainer(MaintainerThread& maintainer);  

protected:    
    
    virtual void main();                                        // główna pętla

private:
    std::vector<DispenserUI*> dispensers;
    std::vector<EmployeeUI*> employees;
    std::vector<MaintainerUI*> maintainers;
    ShelfUI* shelf;

    std::string prepareFrame();
};

#endif
