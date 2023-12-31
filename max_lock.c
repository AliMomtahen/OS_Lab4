#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "max_lock.h"

void
initlock_max(struct max_lock *lk, char *name)
{
  initlock(&lk->lk, "priority lock");
  lk->locked = 0;
  lk->lst_proc = 0;
  lk->pid = 0;
  lk->name = name;
}

void add_to_max_queue(struct max_lock *lk, int id){
    struct max_queue* current = (struct max_queue*)kalloc();
    if (current == 0){
        panic("can't allocate memory");
    }
    current->proc = id;
    struct max_queue* head = lk->lst_proc;
    if (head == 0){
        current->next = 0;
        lk->lst_proc = current;
        return;
    }else{
      if(head->next->proc < current->proc){
        current->next=head;
        lk->lst_proc = current;
        return;
      }
    }
    while(1){
        if (head->next == 0){
            current->next = 0;
            head->next = current;
            break;
        }
        if (head->next->proc > current->proc){
            head = head->next;
        }
        else{
            struct max_queue * temp = head->next;
            head->next = current;
            current->next = temp;
            break;
        }
    }
}

int get_from_queue(struct max_lock *queue){
    if (queue == 0){
        return 0;
    }
    int ans = queue->lst_proc->proc;
    struct max_queue* temp = queue->lst_proc->next;
    if(queue->lst_proc)
      
    queue->lst_proc = temp;

    return ans;
}

void
acquire_max_lock(struct max_lock *lk)
{
  acquire(&lk->lk);
  if (lk->locked) {
    add_to_max_queue(lk, myproc()->pid);
    sleep(lk, &lk->lk);
  }

  lk->locked = 1;
  lk->pid = myproc()->pid;
  release(&lk->lk);
}

void
release_max_lock(struct max_lock *lk)
{
  acquire(&lk->lk);
  //int pp;
  int proc = get_from_queue(lk);
  lk->locked = 0;
  lk->pid = 0;
  //pp = proc->pid;
  if (proc != 0){
    wakeup_max(lk , proc);
    
  }
  release(&lk->lk);
  cprintf("the procces %d relese and pop\n" , myproc()->pid);
}

int
holding_max_lock(struct max_lock *lk)
{
  int r;
  
  acquire(&lk->lk);
  r = lk->locked && (lk->pid == myproc()->pid);
  release(&lk->lk);
  return r;
}
