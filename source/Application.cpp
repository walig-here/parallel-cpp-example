#include <ios>
#include <iostream>

#include "Application.h"
#include "EmployeeThread.h"
#include "MaintainerThread.h"

const char Application::QUIT_CHAR = 'q';
const char Application::NEW_DISPENSER_CHAR = 'd';
const char Application::NEW_EMPLOYEE_CHAR = 'e';
const char Application::NEW_MAINTAINER_CHAR = 'm';

Application* Application::instance = nullptr;

void Application::run()
{
    this->is_running = true;
    this->user_interface.detach();
    this->addDispenser();
    this->addEmployee();
    this->addMaintainer();

    char input;
    while (this->is_running)
    {
        input = getchar();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (input)
        {
        case Application::QUIT_CHAR:
            this->is_running = false;
            break;
        case Application::NEW_EMPLOYEE_CHAR:
            this->addEmployee();
            break;
        case Application::NEW_DISPENSER_CHAR:
            this->addDispenser();
            break;
        case Application::NEW_MAINTAINER_CHAR:
            this->addMaintainer();
            break;
        }
    }
}

Application::Application():
    is_running(false),
    shelf(CoffeeShelf::getInstance()),
    user_interface(*CoffeeShelf::getInstance()) {}

Application::~Application()
{
    for (auto thread : this->threads)
        delete thread;
    for (auto dispenser : this->dispensers)
        delete dispenser;
    delete shelf;
}

Application* Application::getInstance()
{
    if (Application::instance == nullptr)
        Application::instance = new Application();
    return Application::instance;
}

void Application::addEmployee()
{
    EmployeeThread* new_employee = new EmployeeThread();
    this->user_interface.addEmployee(*new_employee);
    this->threads.push_back(new_employee);
    new_employee->detach();
}

void Application::addDispenser()
{
    CoffeeDispenser* dispenser = new CoffeeDispenser();
    this->user_interface.addDispenser(*dispenser);
    this->dispensers.push_back(dispenser);
}

void Application::addMaintainer()
{
    MaintainerThread* new_maintainer = new MaintainerThread();
    this->user_interface.addMaintainer(*new_maintainer);
    this->threads.push_back(new_maintainer);
    new_maintainer->detach();
}
