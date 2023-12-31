#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_get_process_lifetime(void)
{
  int pid;
  if(argint(0, &pid) < 0)
    return -1;
  return get_process_lifetime(pid);
}


int sys_get_uncle_count(void){
  int pid;
  if(argint(0, &pid) < 0)
    return -1;
  return get_uncle_count(pid);
}


int sys_change_proc_queue(void){
  int pid;
  int new_q;
  if(argint(0, &pid) < 0 || argint(1 , &new_q)){
    return -1;
  }
  return change_queue(pid , new_q);
}
int sys_change_param_bjf(void){
  int pid;
  
  float priority_ratio, arrival_time_ratio, executed_cycle_ratio,
    process_size_ratio;
  if(argint(0, &pid) < 0 ||
     argfloat(1, &priority_ratio) < 0 ||
     argfloat(2, &arrival_time_ratio) < 0 ||
     argfloat(3, &executed_cycle_ratio) < 0 || 
     argfloat(4, &process_size_ratio) < 0)
     
     {
      
    return -1;
  }
  return  change_param_proc(pid, priority_ratio, arrival_time_ratio,
        executed_cycle_ratio , process_size_ratio);
}
int sys_change_param_bjf_all(void){
  
  float priority_ratio, arrival_time_ratio, executed_cycle_ratio,
    process_size_ratio;
  if(
     argfloat(0, &priority_ratio) < 0 ||
     argfloat(1, &arrival_time_ratio) < 0 ||
     argfloat(2, &executed_cycle_ratio) < 0 || 
     argfloat(3, &process_size_ratio) < 0)
     
     {
    return -1;
  }
  
  return change_param_of_all(priority_ratio , arrival_time_ratio
          ,executed_cycle_ratio , process_size_ratio);
  
}



int sys_print_info_proc(void){
  //cprintf("Ajaba\n");
  print_info();
  return 0;
}


int sys_test_max_lock(void){
  acquire_max_lock(&testmax_lock);
   
  
  int n=20;
  uint ticks0;

  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  //wakeup(&ticks);
  release_max_lock(&testmax_lock);


  return 0;
}


