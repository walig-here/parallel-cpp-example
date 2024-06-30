#include <iostream>
#include <sstream>

#include "UserInterfaceThread.h"
#include "CofeeDispenser.h"
#include "DispenserUI.h"
#include "EmployeeUI.h"
#include "Application.h"
#include "MaintainerUI.h"
#include "MaintainerThread.h"
#include "ShelfUI.h"


void UserInterfaceThread::main()
{
    std::chrono::duration<double> current_frame_time;
    std::chrono::high_resolution_clock::time_point previous_time;

    while (true)
    {
        current_frame_time = std::chrono::high_resolution_clock::now() - previous_time;
        if(current_frame_time.count() > 0.1)
        {
            std::system("clear");
            std::cout << this->prepareFrame() << std::endl;
            previous_time = std::chrono::high_resolution_clock::now();
        }
    }
}

std::string UserInterfaceThread::prepareFrame()
{
    std::ostringstream frame;
    std::vector<EmployeeUI*> employees(this->employees);
    std::vector<MaintainerUI*> maintainers(this->maintainers);

    for (int i = 0; i < 100; i++)
        frame << "=";

    frame << "\n\nDispensers:\n";
    for(auto dispenser : this->dispensers)
        frame << dispenser->toString(employees, maintainers) << "\n";
    
    frame << "\nWorking zone:\n";
    for (int i = 0; i < employees.size(); i++)
    {
        if (i % 8 == 0)
            frame << "\n";
        frame << employees[i]->toString() << " ";
    }

    frame << "\n\nStorage:\n";
    frame << this->shelf->toString(maintainers);
    frame << "\n";
    for (int i = 0; i < maintainers.size(); i++)
    {
        if (i % 8 == 0)
            frame << "\n";
        frame << maintainers[i]->toString() << " ";
    }
    
    frame << "\n\n";
    for (int i = 0; i < 100; i++)
        frame << "=";
    frame << "\n";

    frame << "[" << Application::QUIT_CHAR << "] quit\t";
    frame << "[" << Application::NEW_DISPENSER_CHAR << "] add dispenser\t";
    frame << "[" << Application::NEW_EMPLOYEE_CHAR << "] add employee\t";
    frame << "[" << Application::NEW_MAINTAINER_CHAR << "] add maintainer\t";

    return frame.str();
}

UserInterfaceThread::UserInterfaceThread(CoffeeShelf& shelf):
    Thread("UserInterface"),
    shelf(new ShelfUI()) 
{
    shelf.addSubscriber(this->shelf);
}

UserInterfaceThread::~UserInterfaceThread()
{
    for (auto dispenser : this->dispensers)
        delete dispenser;
    for (auto employee : this->employees)
        delete employee;
    delete shelf;
}

void UserInterfaceThread::addDispenser(CoffeeDispenser& dispenser)
{
    this->dispensers.push_back(new DispenserUI(
        CoffeeDispenser::getNumberOfDispensers(),
        dispenser.getAmountOfCoffee()
    ));
    dispenser.addSubscriber(this->dispensers.back());
}

void UserInterfaceThread::addEmployee(EmployeeThread& employee)
{
    this->employees.push_back(new EmployeeUI(
        employee.getId(),
        employee.getEnergy(),
        employee.getCurrentDispenserIndex()
    ));
    employee.addSubscriber(this->employees.back());
}

void UserInterfaceThread::addMaintainer(MaintainerThread &maintainer)
{
    this->maintainers.push_back(new MaintainerUI(
        maintainer.getId(),
        maintainer.getMaxCoffee(),
        maintainer.popCoffee(),
        maintainer.getCurrentDispenserIndex()
    ));
    maintainer.addSubscriber(this->maintainers.back());
}
