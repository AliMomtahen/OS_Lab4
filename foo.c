#include "types.h"
#include "user.h"
// #include <stdio.h>

#define PROCS_NUM 10

int main()
{
    printf(1 , "ddd\n");
    for (int i = 0; i < PROCS_NUM; ++i)
    {
        int pid = fork();
        if (pid > 0)
            continue;
        if (pid == 0)
        {
            sleep(2000);
            for (int j = 0; j < 1000 * i; ++j)
            {
                int x = 5;
                for (long k = 0; k < 10000000000; ++k)
                    x++;
            }
            exit();
        }
    }
    while (wait() != -1)
        ;
    exit();
}

// #include "types.h"
// #include "stat.h"
// #include "user.h"


// int main (int argc, char* argv[]) {
//     // set_sys_bjf(2.3,2.1,3.4,5.6);
//     // set_bjf_proc();
//     int pid = fork();
//     if (pid>0){
//         change_proc_queue(pid,3);
//         sleep(1500);
//     }
//     else{
//         sleep(1000);
//         exit();
//         pid = fork();
//         if (pid > 0){
//             char buffer[4];
//             read(0,buffer,4);
//         }
//         else{
//             sleep(10);
//         }
//         exit();
//     }
//     exit();
// }


// int main(int argc, char* argv[]){
//     for (int i = 0; i < 4; i++){
//         int pid = fork();
//         if (pid == 0){
//             sleep(1000);
//             int x = 0;
//             for (long long j = 0; j < 1000000000000; j++){
//                 for (long long k = 0; k < 10000000000000; k++){
//                     x += k * 12 - j;
//                 }
//             }
//             exit();
//         }
//         if(pid == 5)
//             set_bjf_proc(pid,2,3,4,6);

//     }
//     for (int i = 0; i < 4; i++){
//         wait();
//     }
//     exit();
// }

// int main(){
//     int pid;
//     pid = fork();
//     if(pid ==0){
//         // change_queue(myproc()->pid,1);
//         while("true"){
//             ;
//         }
//         exit();
//     }
//     else if(pid >0){
//         wait();
//     }
//     exit();
// }