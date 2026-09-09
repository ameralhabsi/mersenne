#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <gmp.h>
#include <stdbool.h>
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
// given a 64 bit integer, the fucntion checks if it is prime.
// First it checks the first primes from the LUT.
// Then, it tries to divide by these primes to speed up the process.
// Finally, it tries all odd numbers till the square root of
// the number
int is_prime(uint64_t x)
{
    uint32_t k, max;
    max = 1 + (int) sqrt(x);

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

// int nextPrime(uint64_t p){
//     if(p==2)
//         return 3;

//     p+=2;
//     while(!is_prime(p)){
//         p+=2;
//     }
//     return p;
// }   


uint64_t power_mod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (uint64_t)(((__int128_t)result * base) % mod);
        }
        base = (uint64_t)(((__int128_t)base * base) % mod);
        exp /= 2;
    }
    return result;
}

// bool is_prime(uint64_t n) {
//     if (n < 2) return false;
//     if (n == 2 || n == 3) return true;
//     if (n % 2 == 0 || n % 3 == 0) return false;
//     for (uint64_t i = 5; i * i <= n; i += 6) {
//         if (n % i == 0 || n % (i + 2) == 0) return false;
//     }
//     return true;
// }

uint64_t nextExponent() {
    static uint64_t p = 1;

           while (1) {
        p++;
        
        // Step 0: Ensure exponent p is prime
        if (!is_prime(p)) {
            continue;
        }

        // Handle small Mersenne primes directly where M_p fits in standard types
        if (p <= 7) {
            return p;
        }

        // Step 3 (Euler's Theorem): If p ≡ 3 (mod 4) and q = 2p + 1 is prime, 
        // then q divides M_p. For p > 3, q < M_p, so M_p is composite.
        if (p % 4 == 3) {
            uint64_t q = 2 * p + 1;
            if (is_prime(q)) {
                continue; // Composite!
            }
        }

        // Precompute M_p if p < 64 to prevent q from matching or exceeding M_p
        uint64_t Mp = 0;
        if (p < 64) {
            Mp = (1ULL << p) - 1;
        }

        // Steps 1 & 2: Fast Trial Division for q = 2kp + 1
        bool has_factor = false;
        const uint64_t MAX_K = 1000;

        for (uint64_t k = 1; k <= MAX_K; k++) {
            uint64_t q = 2 * k * p + 1;

            // Overflow protection
            if (q < p) break; 

            // STOP if q reaches or exceeds M_p (q cannot be a PROPER factor if q >= M_p)
            if (Mp > 0 && q >= Mp) {
                break;
            }

            // Condition 1: q ≡ ±1 (mod 8)
            uint64_t q_mod8 = q % 8;
            if (q_mod8 != 1 && q_mod8 != 7) {
                continue;
            }

            // Test if q is prime
            if (is_prime(q)) {
                if (power_mod(2, p, q) == 1) {
                    has_factor = true; // Proper factor found; M_p is composite
                    break;
                }
            }
        }

        if (!has_factor) {
            return p;
        }
    }
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

    if(!is_prime(p))
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

