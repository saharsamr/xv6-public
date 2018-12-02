//
// Created by Sahar on 12/2/2018.
//

#ifndef TICKET_LOCK_H
#define TICKET_LOCK_H

#include "defs.h"


struct ticket_lock {
    int ticket;
    int turn;
    int lock;
    struct proc* process;
};

#endif //TICKET_LOCK_H
