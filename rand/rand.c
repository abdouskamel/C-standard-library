#include "rand.h"

/*
 * Linear congruential generator :
 * X0 = seed;
 * Xn+1 = (aXn + c) % m
 */
#define a 1103515245
#define c 12345
#define m RAND_MAX

static unsigned int next = 1;

void srand(unsigned int seed)
{
    next = seed;
}

int rand(void)
{
    next = (a * next + c) % m;
    return (next);
}
