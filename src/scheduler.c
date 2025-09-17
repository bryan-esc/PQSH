/* scheduler.c: PQSH Scheduler */

#include "pqsh/utils.h"
#include "pqsh/scheduler.h"
#include "pqsh/timestamp.h"

#include <errno.h>
#include <sys/wait.h>

/* Global Variables */

Scheduler PQSHScheduler = {
    .policy    = FIFO_POLICY,
    .cores     = 1,
    .timeout   = 250000,
    .event     = EVENT_INPUT,
};

/* Functions */

/**
 * Add new command to waiting queue.
 * @param   s       Pointer to Scheduler structure.
 * @param   command Command string for new Process.
 **/
void scheduler_add(Scheduler *s, const char *command) {
    /* TODO: Implement */
    Process *p = process_create(command);
    queue_push(&s->waiting, p);
    printf("Added process \"%s\" to waiting queue.\n", command);
}

/**
 * Display status of queues in Scheduler.
 * @param   s       Pointer to Scheduler structure.
 * @param   queue   Bitmask specifying which queues to display.
 **/
void scheduler_status(Scheduler *s, int queue) {
    if(s->total_turnaround_time > 0){
        printf("Running = %4lu, Waiting = %4lu, Finished = %4lu, Turnaround = %05.2lf, Response = %05.2lf\n", 
                s->running.size, s->waiting.size, s->finished.size, s->total_turnaround_time, s->total_response_time);
    } else {
        printf("Running = %4lu, Waiting = %4lu, Finished = %4lu, Turnaround = %s, Response = %s\n", 
                s->running.size, s->waiting.size, s->finished.size, "-nan", "-nan");
    }
    /* TODO: Complete implementation. */
    if((queue & RUNNING) == RUNNING) {
        if(s->running.size > 0){
            printf("Running Queue:\n");
            queue_dump(&s->running, stdout);
        }
    } else if((queue & WAITING) == WAITING){
        if(s->waiting.size > 0){
            printf("Waiting Queue:\n");
            queue_dump(&s->waiting, stdout);
        }
    } else if((queue & FINISHED) == FINISHED){
        if(s->finished.size > 0){
            printf("Finished Queue:\n");
            queue_dump(&s->finished, stdout);
        }
    } else {
        if(s->running.size > 0){
            printf("Running Queue:\n");
            queue_dump(&s->running, stdout);
        }
        if(s->waiting.size > 0){
            printf("Waiting Queue:\n");
            queue_dump(&s->waiting, stdout);
        }
        if(s->finished.size > 0){
            printf("Finished Queue:\n");
            queue_dump(&s->finished, stdout);
        }
    }
}

/**
 * Schedule next process using appropriate policy.
 * @param   s       Pointer to Scheduler structure.
 **/
void scheduler_next(Scheduler *s) {
    /* TODO: Dispatch to appropriate scheduler function. */
    if(s->policy == FIFO_POLICY){
       scheduler_fifo(s);
    } else if(s->policy == RDRN_POLICY){
      scheduler_rdrn(s);
    }
}

/**
 * Wait for any children and remove from queues and update metrics.
 * @param   s       Pointer to Scheduler structure.
 **/
void scheduler_wait(Scheduler *s) {
    /* TODO: Wait for any children without blocking:
     *
     *  - Remove process from queues.
     *  - Update Process metrics.
     *  - Update Scheduler metrics.
     **/
     
     pid_t pid;
    

     while((pid = waitpid(-1, NULL, WNOHANG)) > 0){
        Process *found = queue_remove(&s->running, pid);
        if (!found){
            printf("Failed to remove from running queue");
            
        }
        found->end_time = timestamp();
        s->total_response_time += found->end_time - found->start_time;
        s->total_turnaround_time += found->end_time - found -> arrival_time;
        queue_push(&s->finished, found);
     }
}

/**
 * Handle SIGALRM by setting appropriate event.
 * @param   signum  Signal number (ignored).
 **/
void scheduler_handle_sigalrm(int signum) {
    /* TODO: Add EVENT_TIMER to PQSHScheduler. */
    PQSHScheduler.event = EVENT_TIMER;
}

/**
 * Handle SIGCHLD by setting appropriate event.
 * @param   signum  Signal number (ignored).
 **/
void scheduler_handle_sigchld(int signum) {
    /* TODO: Add EVENT_CHILD to PQSHScheduler. */
    PQSHScheduler.event = EVENT_CHILD;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
