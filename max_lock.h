
#pragma once



struct max_lock
{
    unsigned int locked;       // Is the lock held?
    struct spinlock lk; // spinlock protecting this sleep lock
    char *name;        // Name of lock.
    int pid;
    struct max_queue *lst_proc;
};

struct max_queue
{
    int proc;
    struct max_queue* next;
};



void
initlock_max(struct max_lock *lk, char *name);

void add_to_max_queue(struct max_lock *lk, int proc);

int get_from_queue(struct max_lock *queue);

void
acquire_max_lock(struct max_lock *lk);


void
release_max_lock(struct max_lock *lk);


int holding_max_lock(struct max_lock *lk);

