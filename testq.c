#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
    
    for(int i=0 ; i < 10 ; i++){

        int pid = fork();
        if(pid == 0){
            test_max_lock();
            exit();
        }
    }

    for(int i=0 ;i < 10 ; i++){
        wait();
    }
    

    exit();
}
