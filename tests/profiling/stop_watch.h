#ifndef STOP_WATCH
#define STOP_WATCH

#include <time.h>

// https://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/

typedef struct{
    time_t tv_sec; // seconds
    long tv_nsec; // nanoseconds
} timespec;

typedef struct
{
    timespec start;
    timespec stop;
} StopWatch;

void stop_watch_start(StopWatch* watch);
void stop_watch_stop(StopWatch* watch);

timespec time_diff(StopWatch* watch);

#endif
