#include "Arduino.h"
#include "WMath.h"

void randomSeed(unsigned long seed)
{
    if (seed != 0UL) {
        srand((unsigned int)seed);
    }
}

long random(long upperBound)
{
    if (upperBound <= 0L) {
        return 0L;
    }

    return (long)((unsigned long)rand() % (unsigned long)upperBound);
}

long random(long lowerBound, long upperBound)
{
    if (lowerBound >= upperBound) {
        return lowerBound;
    }

    const uint64_t range = (uint64_t)((int64_t)upperBound - (int64_t)lowerBound);
    const uint64_t offset = (uint64_t)(unsigned int)rand() % range;
    return (long)((int64_t)lowerBound + (int64_t)offset);
}

long map(long value, long fromLow, long fromHigh, long toLow, long toHigh)
{
    if (fromHigh == fromLow) {
        return toLow;
    }

    return (long)(((int64_t)value - fromLow) * ((int64_t)toHigh - toLow) /
            ((int64_t)fromHigh - fromLow) + toLow);
}
