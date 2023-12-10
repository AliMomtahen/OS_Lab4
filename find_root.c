#ifndef _FIND_ROOT_C
#define _FIND_ROOT_C




#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"




int find_droot(int n){
    int bef_ebx;
    asm volatile( "movl %%ebx, %0\n\t" 
    "movl %1, %%ebx" 
    : "=r"(bef_ebx) 
    : "r"(n)
    )
    ;
    int r = find_digital_root();
    
    asm volatile( "movl %0, %%ebx" 
        :: "r"(bef_ebx));
    return r;
}



int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    
    int result = find_droot(n);
    if(result <= 0 ){
        write(1 , "the number not have digital root\n" , 34);
    }else{
        
        printf(1 , "root is %d\n" , result);
    }


    exit();
}



#endif