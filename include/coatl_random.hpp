#ifndef COATL_RANDOM_HPP
#define COATL_RANDOM_HPP

namespace Coatl
{
    class Random
    {
        public:
            static void Seed(unsigned int seed);
            static unsigned int UInt();
            static unsigned int UInt(unsigned int max);
            static unsigned int UInt(unsigned int min, unsigned int max);
            static float Float();
    };
}

#endif // COATL_RANDOM_HPP
