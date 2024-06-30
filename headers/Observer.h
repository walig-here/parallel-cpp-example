#ifndef OBSERVER_H
#define OBSERVER_H

#include <iostream>

class Observer
{
public:

    Observer(std::string message_prefix);
    virtual void update(const std::string message) = 0;
    virtual ~Observer() {};

protected:

    std::string message_prefix;
};

#endif
