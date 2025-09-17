/* pqsh.c: Process Queue Shell */

#include "pqsh/utils.h"
#include "pqsh/options.h"
#include "pqsh/scheduler.h"

#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>

/* Global Variables */

extern Scheduler PQSHScheduler;

/* Help Message */

void help() {
    printf("Commands:\n");
    printf("  add    command    Add command to waiting queue.\n");
    printf("  status [queue]    Display status of specified queue (default is all).\n");
    printf("  help              Display help message.\n");
    printf("  exit|quit         Exit shell.\n");
}

/* Main Execution */

int main(int argc, char *argv[]) {
    Scheduler *s = &PQSHScheduler;

    /* TODO: Parse command line options */
    if(!parse_command_line_options(argc, argv, s)){
        help();
        exit(EXIT_FAILURE);
    }

    /* TODO: Register signal handlers */
    struct sigaction sigchild_action = {.sa_handler = scheduler_handle_sigchld }; 
    if(sigaction(SIGCHLD, &sigchild_action, NULL) < 0){
      printf("sigaction child error\n"); 
      exit(EXIT_FAILURE);
    }

    struct sigaction sigalrm_action = {.sa_handler = scheduler_handle_sigalrm};
    if(sigaction(SIGALRM, &sigalrm_action, NULL) < 0){
        printf("sigaction alarm error\n"); 
        exit(EXIT_FAILURE);
    }

    /* TODO: Start timer interrupt */
    struct itimerval interval = {
        .it_interval = { .tv_sec = 0, .tv_usec = s->timeout },
        .it_value = { .tv_sec = 0, .tv_usec = s->timeout },
    };

    if(setitimer(ITIMER_REAL, &interval, NULL) < 0){
        printf("setitimer error\n"); 
        exit(EXIT_FAILURE);     
    }

    /* TODO: Process shell commands */
    while (!feof(stdin)) {
        char command[BUFSIZ]  = "";
        char argument[BUFSIZ] = "";

        printf("\nPQSH> ");

        while (!fgets(command, BUFSIZ, stdin) && !feof(stdin)) {
            /* TODO: Check events */
            if(s->event == EVENT_TIMER){
               scheduler_next(s);
            } else if(s->event == EVENT_CHILD){
               scheduler_wait(s);
            }
            s->event = EVENT_INPUT;
        }

        chomp(command);

        if (streq(command, "help")) {
            help();
        } else if (streq(command, "exit") || streq(command, "quit")) {
            break;
        } else if (sscanf(command, "add %[^\t\n]", argument) == 1) {
            /* TODO: Handle add command */
            scheduler_add(s, command);
        } else if (streq(command, "status") || sscanf(command, "status %[^\t\n]", argument) == 1) {
            /* TODO: Handle status command */
            int queue = atoi(argument);
            if(argument != '\0'){
                to_upper_case(argument);
            }
            if(streq(argument, "RUNNING")){
                queue = RUNNING;
            } else if(streq(argument, "WAITING")){
                queue = WAITING;
            } else if(streq(argument, "FINISHED")){
                queue = FINISHED;
            }
            scheduler_status(s, queue);
        } else if (strlen(command)) {
            printf("Unknown command: %s\n", command);
        }
    }

    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
