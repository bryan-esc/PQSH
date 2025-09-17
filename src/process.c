/* process.c: PQSH Process */

#include "pqsh/utils.h"
#include "pqsh/process.h"
#include "pqsh/timestamp.h"

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

/**
 * Create new process structure given command.
 * @param   command     String with command to execute.
 * @return  Pointer to new process structure
 **/
Process *process_create(const char *command) {
    /* TODO: Implement */
    Process *p = calloc(1, sizeof(Process));
    snprintf(p->command, sizeof p->command, "%s", command);
    p->arrival_time = timestamp();
    return p;
}

/**
 * Start process by forking and executing the command.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not starting the process was successful
 **/
bool process_start(Process *p) {
    /* TODO: Implement */
    pid_t pid = fork();
    p->start_time = timestamp();

    if(pid == 0){
        char *argv[MAX_ARGUMENTS] = {0};
        size_t i = 0;
        for(char *token = strtok(p->command, " "); token; token = strtok(NULL, " ")){
            argv[i++] = token;
        }
        execvp(argv[0], argv);
        exit(0);
    } else {
      p->pid = pid;
      return true;
    }
    return false;
}

/**
 * Pause process by sending it the appropriate signal.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not sending the signal was successful.
 **/
bool process_pause(Process *p) {
    /* TODO: Implement */
    if(kill(p->pid, SIGSTOP) == -1){
      return false;
    }
    return true;
}

/**
 * Resume process by sending it the appropriate signal.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not sending the signal was successful.
 **/
bool process_resume(Process *p) {
    /* TODO: Implement */
    if(kill(p->pid, SIGCONT) == -1){
      return false;
    }
    return true;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
