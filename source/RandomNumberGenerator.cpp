#include "RandomNumberGenerator.h"

int RandomNumberGenerator::getInt(int min, int max)
{
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(this->random_engine);
}