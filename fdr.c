#ifndef _FDR_C
#define _FDR_C

#include "types.h"
#include "defs.h"
#include "param.h"
#include "traps.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"



int find_dr(int n){
    if(n <=0 )
        return -1;
    if( n < 10)
        return n;
    int sum=0;
    while( n > 0 ){
        sum += (n % 10);
        n = n / 10;
    }
    
    
    return find_dr(sum);
}



int sys_find_digital_root(){
    return find_dr(myproc()->tf->ebx);
}



#endif