#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include "mersenne.h"

#define MAX 20000


//---------------------------------------------------------------
void* worker_thread(void* vargp)
{
    uint64_t i;
    int r;

    while((i = nextExponent())<MAX){
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
