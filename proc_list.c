//
// Created by Sahar on 11/22/2018.
//

#include "proc_list.h"
#include "defs.h"

void init_proc_list(struct proc_list *list){
    list->pid = NULL;
}

struct proc_list* find_last(struct proc_list *head){
    struct proc_list *node = head;
    while(node->next != NULL){
        node = node->next;
    }
    return node;
}

void add_to_list(struct proc_list *head, struct proc_list *new_element){
    struct proc_list *last = find_last(head);
    last->next = new_element;
    new_element->prev = last;
    new_element->next = NULL;
}
