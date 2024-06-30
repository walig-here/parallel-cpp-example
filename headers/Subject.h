#ifndef SUBJECT_H
#define SUBJECT_H

#pragma once
#include <vector>

#include "Observer.h"


class Subject
{
public:

    virtual ~Subject() {};
    void addSubscriber(Observer* subscriber);
    void removeSubscriber(Observer* subscriber);
    void nofityAll(const std::string message);

protected:
    Subject();

private:

    std::vector<Observer*> subscribers;
};

#endif
