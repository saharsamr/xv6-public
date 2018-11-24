//
// Created by Sahar on 11/22/2018.
//

#include "spinlock.h"

struct mutex{
    struct spinlock lock;
    int pid; //The id of the process that currently held the lock.
    struct proc_list *waiting_list_head; //Head of the list that hold the waiting processes.
};
