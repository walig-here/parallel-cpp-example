#include "CoffeeShelf.h"
#include "ShelfUI.h"

CoffeeShelf* CoffeeShelf::instance = nullptr;

CoffeeShelf::CoffeeShelf(/* args */)
{
}

CoffeeShelf::~CoffeeShelf()
{
}

CoffeeShelf* CoffeeShelf::getInstance()
{
    if (CoffeeShelf::instance == nullptr)
        CoffeeShelf::instance = new CoffeeShelf();
    return CoffeeShelf::instance;
}

void CoffeeShelf::takeCofee(MaintainerThread &maintainer)
{   
    this->nofityAll(ShelfUI::QUEUE_THREAD + " " + maintainer.getId());

    this->shelf_access_mutex.lock();
    while (maintainer.pushCoffee());
    this->shelf_access_mutex.unlock();

    this->nofityAll(ShelfUI::DEQUEUE_THREAD + " " + maintainer.getId());
}
