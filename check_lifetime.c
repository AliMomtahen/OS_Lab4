#ifndef _CHECK_LIFETIME_C
#define _CHECK_LIFETIME_C



#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


int main(){

    if(fork() == 0){
        sleep(1000);
        printf(1 , "child time is : %d \n" , get_process_lifetime(getpid()));
    }else{
        wait();
        printf(1 , "father time is : %d \n" , get_process_lifetime(getpid()));
        exit();
    }

    exit();
}



#endif