//
// Created by Sahar on 11/24/2018.
//

#include <sys/time.h>
#include "defs.h"
#include "ticketlock.h"
#include "proc_list.h"
#include "proc.h"

void intit_ticket_lock(struct ticket_lock *lock, int initial_val){
    pushcli();
    lock->id = time(NULL);
    lock->initial_value = initial_val;
    lock->num_of_available_locks = initial_val;
    lock->generated_ticket_num = 0;
    lock->current_ticket_num = 0;
    popcli();
}

void require_ticket_lock(struct ticket_lock *lock){
    pushcli();
    struct proc *p = myproc();
    if(!is_in_list(lock->waiting_processes, p)) {
        lock->generated_ticket_num++;
        give_ticket_to_proc(lock);
    }
    struct proc_list *new_elelment;
    new_elelment->pid = p->pid;
    if(lock->num_of_available_locks > 0)
        if(proc_has_ticket(p, lock)) {
            lock->current_ticket_num++;
            add_to_list(lock->lock_owners_processes, new_elelment);
            lock->num_of_available_locks--;
            remove_from_list(p, lock->waiting_processes);
        }
    else{
        add_to_list(lock->waiting_processes, new_elelment);
        p->state = SLEEPING;
        sched();
    }
    popcli();
}

void release_ticket_lock(struct ticket_lock *lock){
    pushcli();
    struct proc *p = myproc();
    if(is_in_list(lock->lock_owners_processes, p)){
        remove_from_list(p, lock->lock_owners_processes);
        wake_up_next_process_ticketlock(lock);
        lock->num_of_available_locks++;
    }
    popcli();
}

void give_ticket_to_proc(struct ticket_lock *lock){
    pushcli();
    struct proc *p = myproc();
    struct proc_ticket *tckt;
    tckt->lock_id = lock->id;
    tckt->ticket = lock->generated_ticket_num;
    add_ticket(p, tckt);
    popcli();
}

int proc_has_ticket(struct proc *p, struct ticket_lock *lock){
    struct proc_ticket *temp = p->ticket_locks_head;
    while(temp->next != NULL)
        if(
            temp->ticket == lock->current_ticket_num+1 &&
            temp->lock_id == lock->id
        )
            return 1;
    return 0;
}

int is_in_list(struct proc_list *head, struct proc *p){
    struct proc_list *temp = head;
    while(temp->next != NULL)
        if(temp->pid == p->pid)
            return 1;
    return 0;
}

void remove_from_list(struct proc *p, struct proc_list *head){
    struct proc_list *temp = head;
    while(temp->next != NULL)
        if(temp->pid == p->pid){
            struct proc_list *help = temp;
            temp->next != NULL ?
            (temp->prev != NULL ?
                (temp->prev->next = temp->next) :
                (temp->next->prev = temp->prev))
            :
            (temp->prev != NULL ?
             (temp->prev->next = temp->next) : (head = NULL));
            return;
        }
}