#include "acc_testsuite.h"
#ifndef T1
//T1:parallel,data,executable-data,data-region,V:3.0-3.1
int test1(){
    int err = 0;
    srand(SEED);

    real_t * a = (real_t *)malloc(n * sizeof(real_t));
    real_t * b = (real_t *)malloc(n * sizeof(real_t));

    for (int x = 0; x < n; ++x){
        a[x] = rand() / (real_t)(RAND_MAX / 10);
        b[x] = 0.0;
    }
 
    #pragma acc data copyin(a[0:n])
    {
        #pragma acc parallel copyout(c[0:n])
        {
            #pragma acc loop
            {
                for (int x = 0; x < n; ++x){
                  b[x] += a[x];
                }
            }
        }
    }

    for (int x = 0; x < n; ++x){
        if(fabs(a[x] - b[x]) > PRECISION){
                err += 1;
                break;
        }
    }

    return err;
}
#endif

#ifndef T2
//T2:runtime,data,executable-data,construct-independent,V:3.0-3.1
int test2(){
    int err = 0;
    srand(SEED);

    real_t * a = (real_t *)malloc(n * sizeof(real_t));
    real_t * b = (real_t *)malloc(n * sizeof(real_t));

    for (int x = 0; x < n; ++x){
        a[x] = rand() / (real_t)(RAND_MAX / 10);
        b[x] = 0.0;
    }

    #pragma acc data copyin(a[0:n])
    {
        #pragma acc parallel copyout(c[0:n])
        {
            #pragma acc loop
            {
                for (int x = 0; x < n; ++x){
                  b[x] = 0.0;
                }
            }
            #pragma acc loop
            {
                for (int x = 0; x < n; ++x){
                  b[x] += a[x];
                }
            }
        }
    }

    for (int x = 0; x < n; ++x){
        if(fabs(a[x] - b[x]) > PRECISION){
                err += 2;
                break;
        }
    }

    return err;
}
#endif

#ifndef T3
//T3:runtime,data,executable-data,construct-independent,V:3.0-3.1
int test3(){
    int err = 0;
    srand(SEED);

    real_t * a = (real_t *)malloc(n * sizeof(real_t));
    real_t * b = (real_t *)malloc(n * sizeof(real_t));

    for (int x = 0; x < n; ++x){
        a[x] = rand() / (real_t)(RAND_MAX / 10);
        b[x] = 0;
    }

    #pragma acc data copyin(a[0:n])
    {
        #pragma acc parallel copyout(zero: c[0:n])
        {
            #pragma acc loop
            {
                for (int x = 0; x < n; ++x){
                  b[x] += a[x];
                }
            }
        }
    }

    for (int x = 0; x < n; ++x){
        if(fabs(a[x] - b[x]) > PRECISION){
                err += 3;
                break;
        }
    }

    return err;
}
#endif

int main(){
    int failcode = 0;
    int testrun;
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
        failcode = failcode + (1 << 0);
    }
#endif
#ifndef T3
    failed = 0;
    for (int x = 0; x < NUM_TEST_CALLS; ++x){
        failed = failed + test3();
    }
    if (failed != 0){
        failcode = failcode + (1 << 0);
    }
#endif
    return failcode;
}
