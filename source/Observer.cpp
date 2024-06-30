#include "Observer.h"

Observer::Observer(std::string message_prefix)
{
    this->message_prefix = message_prefix + ":";
}