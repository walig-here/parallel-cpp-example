#include "MaintainerUI.h"
#include <sstream>

const std::string MaintainerUI::SET_COFFEE = "set-coffee";
const std::string MaintainerUI::SET_INDEX = "set-index";

MaintainerUI::MaintainerUI( 
    const std::string &id,      
    const int &max_coffee, 
    const int &current_coffee,  
    const int &current_dispenser_index
):
    Observer("mnt"),
    id(id),
    max_coffee(max_coffee), 
    current_cofeee(current_cofeee),
    current_dispenser_index(current_dispenser_index) {}

MaintainerUI::~MaintainerUI() {}

void MaintainerUI::update(const std::string message)
{
    std::istringstream parser(message);
    std::string menmonic;
    parser >> menmonic;

    if (menmonic == MaintainerUI::SET_INDEX)
    {
        parser >> this->current_dispenser_index;
    }
    else if (menmonic == MaintainerUI::SET_COFFEE) 
    {
        parser >> this->current_cofeee;
    }
}

std::string MaintainerUI::toString()
{
    std::ostringstream result;
    result << "<" << this->id << "|" << this->current_cofeee << "/" << this->max_coffee << ">"; 
    return result.str();
}
