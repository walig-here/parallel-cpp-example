#ifndef EMPLOYEE_UI
#define EMPLOYEE_UI

#include <mutex>

#include "EmployeeThread.h"


class EmployeeUI : public Observer
{
public:
    static const std::string SET_INDEX;
    static const std::string SET_ENERGY;

    EmployeeUI(const std::string& id, const int& energy, const int& current_dispenser_index);
    ~EmployeeUI();
    virtual void update(const std::string message);
    std::string toString();
    int getDispenserIndex() { return this->current_dispenser_index; }
    std::string getId() { return id; }
    int getEnergy() { return energy; }

private:
    int current_dispenser_index;
    std::string id;
    int energy;
};

#endif
