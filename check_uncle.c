#ifndef _CHECK_UNCLE_C
#define _CHECK_UNCLE_C


#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


int main(){

    if(fork() == 0){
        sleep(500);
        
    }else{
        if(fork() == 0){
            sleep(500);
            
        }else{

            if(fork() == 0){
                if(fork() == 0){
                    printf(1 , "uncle number : %d\n" , get_uncle_count(getpid()));
                }else{
                    wait();
                    exit();
                }
                exit();
            }else{
                wait();
                wait();
                wait();
                
                exit();
            }

            exit();

        }
        
        wait();

        exit();
    }

    exit();
    

}



#endif