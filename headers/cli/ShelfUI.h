#ifndef SHELF_UI_H
#define SHELF_UI_H

#include "CoffeeShelf.h"
#include "Observer.h"
#include "MaintainerUI.h"

class ShelfUI : public Observer
{
public:
    static const std::string QUEUE_THREAD;
    static const std::string DEQUEUE_THREAD;

    ShelfUI();
    ~ShelfUI();
    std::string toString(std::vector<MaintainerUI*>& maintainers);
    virtual void update(const std::string message);

private:
    std::vector<std::string> queue_to_shelf; 
};

#endif