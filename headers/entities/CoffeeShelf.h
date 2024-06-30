#ifndef COFFEE_SHELF_H
#define COFFEE_SHELF_H

#include <mutex>
#include "MaintainerThread.h"
#include "Subject.h"

class CoffeeShelf : public Subject
{
public:
    static CoffeeShelf* getInstance();

    void takeCofee(MaintainerThread& maintainer);
    ~CoffeeShelf();

private:
    static CoffeeShelf* instance;

    std::mutex shelf_access_mutex;    

    CoffeeShelf();
};

#endif
