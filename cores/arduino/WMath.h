#ifndef W_MATH_H
#define W_MATH_H

void randomSeed(unsigned long seed);
long random(long upperBound);
long random(long lowerBound, long upperBound);
long map(long value, long fromLow, long fromHigh, long toLow, long toHigh);

#endif
