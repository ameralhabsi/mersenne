#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <gmp.h>

#define N 168

uint32_t primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67,
 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163,
 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263,
 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367,
 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587,
 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683,
 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809,
 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911,
 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997};

//---------------------------------------------------------------
// given a 64 bit numbers, the fucntion checks if it prime
// first it checks the first primes from the LUT.
// Then, it divides by these primes to speed up the process.
// Finally, it tries all odd numbers till the square root of
// the number
int isPrime(uint64_t x)
{
    uint32_t k, max;
    max = 1 + (int) floor(sqrt(x));

    for(k=0; k<N; k++){
        if(x==primes[k])
            return 1;

        if((x % primes[k])==0)
            return 0;    
    }

    for(k=primes[N-1]; k<max; k+=2){
        if(x%k==0)
            return 0;
    }

    return 1;
}

//---------------------------------------------------------------
// checks if exponent p leads to a Mersenne prime 2^p-1. It uses
// Lucas-Lehmer test
// return 1 if 2^p-1 is prime, or 0 otherwise
//
int isMersennePrime(uint64_t p)   // 
{
    mpz_t s, M;
    uint64_t i;

    if(p==2)
        return 1;

    if(!isPrime(p))
        return 0;     // if exponent is not prime the 2^p-1 cannot be prime

    // Lucas Lehmer test
    mpz_init(s);
    mpz_init(M);

    mpz_set_ui(s, 4);
    
    mpz_ui_pow_ui(M, 2, p);
    mpz_sub_ui(M, M, 1);

    for(i=0; i<p-2; i++){
        mpz_mul(s, s, s);
        mpz_sub_ui(s, s, 2);
        mpz_mod(s, s, M);
    }

    if(mpz_sgn(s)==0)
        return 1;

    return 0;
}

