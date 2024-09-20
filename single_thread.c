#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "mersenne.h"

int main()
{
    uint64_t i;
    int r;

    for(i = 2; i<650; i++){
        r = isMersennePrime(i);

        if(r)
            printf("2^%ld-1 is Mersenne prime\n", i);
    }

    return 0;
}