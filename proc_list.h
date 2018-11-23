//
// Created by Sahar on 11/22/2018.
//

struct proc_list{
    int pid;
    struct proc_list *next;
    struct proc_list *prev;
};