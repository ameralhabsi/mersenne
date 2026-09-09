#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "mersenne.h"

int main()
{
    uint64_t p=1;
    int r;

    while(p<21702){
        p=nextExponent();
        r = isMersennePrime(p);

        if(r)
            printf("2^%ld-1 is Mersenne prime\n", p);
    }

    return 0;
}