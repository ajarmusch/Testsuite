#include "acc_testsuite.h"
#ifndef T1
//T1:atomic,construct-independent,V:2.7-3.2
int test1(){
    int err = 0;
    srand(SEED);
    real_t *a = (real_t *)malloc(n * sizeof(real_t));
    real_t *b = (real_t *)malloc(n * sizeof(real_t));

    for (int x = 0; x < n; ++x){
        a[x] = rand() / (real_t)(RAND_MAX / 10);
        b[x] = rand() / (real_t)(RAND_MAX / 10);
    }

    #pragma acc data copy(a[0:n], b[0:n])
    {
        #pragma acc parallel
        {
	    #pragma acc loop independent
            for (int x = 0; x < n; ++x){
                #pragma acc atomic
                    a[x] = a[x] * 2;
                    b[x] = a[x];
            }
        }
    }

    for (int x = 0; x < n; ++x){
        if (fabs(a[x] - b[x]) > PRECISION){
            err += 1;
        }
    }

    return err;
}
#endif

#ifndef T2
//T2:parallel,loop,combined-constructs,V:2.7-3.2
int test2(){
    int err = 0;
    srand(SEED);
    real_t * a = (real_t *)malloc(n * sizeof(real_t));
    real_t * a_copy = (real_t *)malloc(n * sizeof(real_t));

    for (int x = 0; x < n; ++x){
        a[x] = rand() / (real_t)(RAND_MAX / 10);
        a_copy[x] = a[x];
    }

    #pragma acc data copy(a[0:n])
    {
        #pragma acc parallel 
        {
            #pragma acc loop independent
                for (int x = 1; x < n; ++x){
                    #pragma acc atomic
                    a[x] = a[x - 1] + a[x];
                }
        }
    }

    real_t rolling_total = 0.0;
    for (int x = 0; x < n; ++x){
        rolling_total += a_copy[x];
        if (fabs(rolling_total - a[x]) > PRECISION){
            err = 1;
        }
    }
    return err;
}
#endif

int main(){
    int failcode = 0;
    int failed;
#ifndef T1
   failed = 0;
   for (int x = 0; x < NUM_TEST_CALLS; ++x){
       failed = failed + test1();
   }
   if (failed != 0){
       failcode = failcode + (1 << 0);
   }
#endif
#ifndef T2
    failed = 0;
    for (int x = 0; x < NUM_TEST_CALLS; ++x){
        failed = failed + test2();
    }
    if (failed != 0){
        failcode = failcode + (1 << 1);
    }
#endif
    return failcode;
}

