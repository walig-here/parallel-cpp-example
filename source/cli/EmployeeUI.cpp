#include <sstream>

#include "EmployeeUI.h"

const std::string EmployeeUI::SET_INDEX =  "set-index";
const std::string EmployeeUI::SET_ENERGY = "set-energy";

EmployeeUI::EmployeeUI(const std::string& id, const int& energy, const int& current_dispenser_index): 
    Observer("employee"),
    id(id),
    energy(energy),
    current_dispenser_index(current_dispenser_index) {}

EmployeeUI::~EmployeeUI()
{
}

void EmployeeUI::update(const std::string message)
{
    std::istringstream parser(message);
    std::string menmonic;
    parser >> menmonic;

    if (menmonic == EmployeeUI::SET_INDEX)
        parser >> this->current_dispenser_index;
    else if (menmonic == EmployeeUI::SET_ENERGY) 
        parser >> this->energy;
}

std::string EmployeeUI::toString()
{
    std::ostringstream result;
    result << "(" << this->id << "|" << this->energy << ")"; 
    return result.str();
}
