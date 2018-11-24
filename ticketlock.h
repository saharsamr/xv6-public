//
// Created by Sahar on 11/24/2018.
//

#include <sys/types.h>
#include "defs.h"

struct ticket_lock{
    time_t id;
    int initial_value;
    int num_of_available_locks;
    struct proc_list *waiting_processes;
    struct proc_list *lock_owners_processes;
    int generated_ticket_num;
    int current_ticket_num;
};
