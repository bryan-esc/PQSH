/* queue.c: PQSH Queue */

#include "pqsh/utils.h"
#include "pqsh/queue.h"

#include <assert.h>

/**
 * Push process to back of queue.
 * @param q     Pointer to Queue structure.
 **/
void queue_push(Queue *q, Process *p) {
    /* TODO: Implement */
    p->next = NULL;
    if(!q->tail){
      q->head = p;
    } else {
      q->tail->next = p;
    }
    q->tail = p;
    q->size++;
}

/**
 * Pop process from front of queue.
 * @param q     Pointer to Queue structure.
 * @return  Process from front of queue.
 **/
Process * queue_pop(Queue *q) {
    /* TODO: Implement */
    
    if(!q->head){
       return NULL;
    }

    Process *temp = q->head;
    if (!q->head->next){
      q->head = NULL;
      q->tail = NULL;
      q->size--;
      return temp;
    }

    q->head = q->head->next;
    temp->next = NULL;
    q->size--;

    return temp;
}

/**
 * Remove and return process with specified pid.
 * @param q     Pointer to Queue structure.
 * @param pid   Pid of process to return.
 * @return  Process from Queue with specified pid.
 **/
Process * queue_remove(Queue *q, pid_t pid) {
    /* TODO: Implement */
    if(!q->head){
       return NULL;
    }

    Process *curr = q->head;
    Process *prev = NULL;
    while(curr){
       if(curr->pid == pid){
         if(prev){
           prev->next = curr->next;
         } else {
           q->head = curr->next;
         }

         if(curr == q->tail){
            q->tail = prev;
         }

         curr->next = NULL;
         q->size--;
         return curr;
       }
       prev = curr;
       curr = curr->next;
    }
    return NULL;
}

/**
 * Dump the contents of the Queue to the specified stream.
 * @param q     Queue structure.
 * @param fs    Output file stream.
 **/
void queue_dump(Queue *q, FILE *fs) {
    fprintf(fs, "%9s %-30s %-13s %-13s %-13s\n",
            "PID", "COMMAND", "ARRIVAL", "START", "END");
    /* TODO: Display information for each item in Queue. */
    Process *temp = q->head;
    while(temp){
        fprintf(fs, "%9d %-30s %-13lf %-13lf %-13lf\n",
                temp->pid, temp->command, temp->arrival_time, temp->start_time, temp->end_time);
        temp = temp->next;
    }
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
