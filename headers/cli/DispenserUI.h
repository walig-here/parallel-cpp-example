#ifndef DISPENSER_UI_H
#define DISPENSER_UI_H

#include <mutex>

#include "CofeeDispenser.h"
#include "EmployeeUI.h"


class DispenserUI : public Observer
{
public:
    static const std::string SET_COFFEE;
    static const std::string QUEUE_THREAD;
    static const std::string DEQUEUE_THREAD;
    static const std::string MOVE_TO_QUEUE_HEAD;
    static const std::string SET_BROKEN;

    DispenserUI(const int& dispenser_index, const int& coffee);
    ~DispenserUI();
    std::string toString(std::vector<EmployeeUI*>& employees, std::vector<MaintainerUI*>& maintainers);
    int getCoffee() { return this->coffee; }
    int getIndex() { return this->dispenser_index; }
    virtual void update(const std::string message);

private:
    int coffee;
    int dispenser_index;
    std::vector<std::string> queue_to_dispenser; 
    std::mutex queue_to_dispenser_mutex;
    bool is_broken;
};

#endif
