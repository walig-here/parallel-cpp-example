#include <algorithm>

#include "Subject.h"


void Subject::addSubscriber(Observer* subscriber)
{
    this->subscribers.push_back(subscriber);
}

void Subject::removeSubscriber(Observer* subscriber)
{
    auto observer = std::remove(
        subscribers.begin(),
        subscribers.end(),
        subscriber
    );
    this->subscribers.erase(observer, this->subscribers.end());
}

void Subject::nofityAll(const std::string message)
{
    for (auto subscriber : this->subscribers)
        subscriber->update(message);
}

Subject::Subject()
{
}
