#ifndef RANDOM_NUMBER_GENERATOR_H
#define RANDOM_NUMBER_GENERATOR_H

#include <random>

class RandomNumberGenerator
{
public:
    int getInt(int min=INT32_MIN, int max=INT32_MAX);

private:
    std::random_device random_engine;
};

#endif
