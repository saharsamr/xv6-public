//
// Created by Sahar on 11/22/2018.
//

#include "defs.h"
#include "mutex.h"
#include "proc.h"

#define LOCKED 1
#define NOT_LOCKED 0

void init_mutex(struct mutex *lock, char *name){
    pushcli();
    initlock(&lock->lock, name);
    lock->waiting_list_head = NULL;
    popcli();
}

void aquire_mutex(struct mutex *mutex_){ //TODO: hamash kollo malloc mikad.
    pushcli();
    if(mutex_->lock.locked == NOT_LOCKED){
        struct proc *p = myproc();
        mutex_->pid = p->pid;
        mutex_->lock.locked = LOCKED;
        p->state = RUNNABLE;
    }
    else if(mutex_->lock.locked == LOCKED){
        struct proc *p = myproc();
        struct proc_list *new_element;
        new_element->pid = p->pid;
        add_to_list(mutex_->waiting_list_head, new_element);
        p->state = SLEEPING;
        sched();
    }
    popcli();
}

void release_mutex(struct mutex *mutex_){
    pushcli();
    struct proc *p = myproc();
    if(mutex_->pid == p->pid){
        mutex_->lock.locked = NOT_LOCKED;
    }
    popcli();
}
