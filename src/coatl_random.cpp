#include "coatl_pch.hpp"

#include "coatl_random.hpp"

namespace Coatl
{
    void Random::Seed(unsigned int seed)
    {
        srand(seed);
    }

    unsigned int Random::UInt()
    {
        return rand();
    }

    unsigned int Random::UInt(unsigned int max)
    {
        return rand() % max;
    }

    unsigned int Random::UInt(unsigned int min, unsigned int max)
    {
        return (rand() % (max - min + 1)) + min;
    }

    float Random::Float()
    {
        return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }
}
