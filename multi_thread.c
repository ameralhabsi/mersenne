#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include "mersenne.h"

#define MAX 10000

//---------------------------------------------------------------
// provides the next number to be tested as exponent. Threads request these,
// perform calculations and then request the next one. The first prime, 2, 
// is intentionally omited.

uint64_t get_next()
{
    static uint64_t p=2;
    // p = p + 2;

    // if(p>MAX)
    //     return 0;

    // return p;

    if(p==2){
        p=1;
        return 2;
    }

    p = p + 2;
    if(p>MAX)
        return 0;

    return p;

}

//---------------------------------------------------------------
void* worker_thread(void* vargp)
{
    uint64_t i;
    int r;

    while((i = get_next())){
        r = isMersennePrime(i);

        if(r)
            printf("2^%ld-1 is Mersenne prime\n", i);
    }
    
    return NULL;
}

//---------------------------------------------------------------

#define N_THREADS 8

int main()
{
    int i;
    
    pthread_t thread_id[N_THREADS];
    
    for(i=0; i < N_THREADS; i++){
        pthread_create(&thread_id[i], NULL, worker_thread, NULL);
       
    }
     for(i=0; i < N_THREADS; i++){
       
        pthread_join(thread_id[i], NULL);
    }


    return 0;
}