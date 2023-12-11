#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

// #include <stdio.h>



int main()
{
    write(1 , "in print\n" , 10);
    print_info_proc();
    
    exit();
}
