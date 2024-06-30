#ifndef APPLICATION_H
#define APPLICATION_H

#include <list>

#include "UserInterfaceThread.h"
#include "CofeeDispenser.h"
#include "CoffeeShelf.h"


class Application
{
public:

    static const char QUIT_CHAR;
    static const char NEW_EMPLOYEE_CHAR;
    static const char NEW_DISPENSER_CHAR;
    static const char NEW_MAINTAINER_CHAR;

    static Application* getInstance();

    ~Application();
    void run();

private:

    static Application* instance;

    std::list<CoffeeDispenser*> dispensers;
    UserInterfaceThread user_interface;
    CoffeeShelf* shelf;
    bool is_running;
    std::list<Thread*> threads;

    Application();
    void addEmployee();
    void addDispenser();
    void addMaintainer();
};

#endif


