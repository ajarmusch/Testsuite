#include "acc_testsuite.h"
#ifndef T1
//T1:shutdown,V:3.0-3.1
int test1(){
    int err = 0;
    srand(SEED);

    #pragma acc wait(1) if(1 == 1)

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
    return failcode;
}
