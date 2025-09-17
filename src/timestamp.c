/* timestamp.c: PQSH Timestamp */

#include "pqsh/timestamp.h"

#include <time.h>
#include <sys/time.h>

/**
 * Return current timestamp as a double.
 *
 * Utilizes clock_gettime for precision (accounting for seconds and
 * nanoseconds) and falls back to time (only account for seconds) if that
 * fails.
 *
 * @return  Double representing the current time.
 **/
double timestamp() {
    /* TODO: Implement */
    struct timespec ts;
    if(clock_gettime(CLOCK_REALTIME, &ts) == 0){
      return (double)ts.tv_sec+(double)ts.tv_nsec/1e9;
    } else {
      return (double)time(NULL);
    }
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
