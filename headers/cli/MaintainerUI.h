#ifndef MAINTAINER_UI_H
#define MAINTAINER_UI_H

#include "Observer.h"

class MaintainerUI : public Observer
{
public:
    static const std::string SET_COFFEE;
    static const std::string SET_INDEX;

    MaintainerUI(
        const std::string& id, 
        const int& max_coffee, 
        const int& current_coffee, 
        const int& current_dispenser_index
    );
    ~MaintainerUI();
    virtual void update(const std::string message);
    std::string toString();
    std::string getId() { return id; }

private:
    int current_cofeee;
    const int max_coffee;
    int current_dispenser_index;
    std::string id;
};

#endif
