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

void add_to_max_queue(struct max_lock *lk, struct proc* proc){
    struct max_queue* current = (struct max_queue*)kalloc();
    if (current == 0){
        panic("can't allocate memory");
    }
    current->proc = proc;
    struct max_queue* head = lk->lst_proc;
    if (head == 0){
        current->next = 0;
        lk->lst_proc = current;
        return;
    }
    while(1){
        if (head->next == 0){
            current->next = 0;
            head->next = current;
            break;
        }
        if (head->next->proc->pid < current->proc->pid){
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

struct proc* get_from_queue(struct max_queue *queue){
    if (queue == 0){
        return 0;
    }
    struct proc* ans = queue->proc;
    struct max_queue* temp = queue->next;
    kfree((char *)queue);
    queue = temp;

    return ans;
}

void
acquire_max_lock(struct max_lock *lk)
{
  acquire(&lk->lk);
  if (lk->locked) {
    add_to_max_queue(lk, myproc());
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
  struct proc* proc = get_from_queue(lk->lst_proc);
  lk->locked = 0;
  lk->pid = 0;
  if (proc != 0){
    wakeup(proc);
  }
  release(&lk->lk);
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
