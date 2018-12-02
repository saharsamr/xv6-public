//
// Created by Sahar on 12/2/2018.
//

#include "ticket_lock.h"
#include "defs.h"
#include "x86.h"

void ticket_lock_init(struct ticket_lock* lock){
    pushcli();
    lock->lock = 0;
    lock->ticket = 0;
    lock->turn = 1;
    lock->process = 0;
    popcli();
}

void acquire_ticket_lock(struct ticket_lock* lock){
    pushcli();
    struct proc* p = myproc();

    if(holding_ticket_lock(lock))
        panic("already acquired.");

    int proc_turn = fetch_add(&lock->ticket, 1);

    while(proc_turn != lock->turn)
        sleep_on_tlock(lock, lock);

    lock->process = p;
    popcli();
}

void release_ticket_lock(struct ticket_lock* lock){
    pushcli();
    if(!holding_ticket_lock(lock))
        panic("Must be the owner of lock to be able to release it!");

    lock->turn = fetch_add(&lock->turn, 1);
    wakeup(lock);
    lock->process = 0;
    popcli();
}

int holding_ticket_lock(struct ticket_lock* lock){
    struct proc* p = myproc();
    return (lock->process == p);
}

void ticketlocktest(struct ticket_lock* lock){
    acquire_ticket_lock(lock);
    release_ticket_lock(lock);
}

