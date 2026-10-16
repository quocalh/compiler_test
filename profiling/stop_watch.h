#ifndef STOP_WATCH
#define STOP_WATCH

#include <time.h>
#include <stdio.h>
typedef struct timespec timespec;

// https://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/


struct timespec{
    time_t tv_sec; // seconds (a second)
    long tv_nsec; // nanoseconds (10^-9 seconds)
};

typedef struct
{
    timespec start;
    timespec stop;
} StopWatch;

void stop_watch_start(StopWatch* watch);
void stop_watch_stop(StopWatch* watch);

timespec time_diff(StopWatch* watch);

static inline void __stop_watch_wrapper_time_diff(StopWatch* watch, struct timespec* out_diff)
{
    timespec diff = time_diff(watch);
    out_diff->tv_sec = diff.tv_sec;
    out_diff->tv_nsec = diff.tv_nsec;
}
typedef struct{
    StopWatch watch; struct timespec diff; int run;
} __macro_watch_para;
#define STOPWATCH \
    for ( \
        __macro_watch_para _sw = {.run = 1}; \
        _sw.run ? (stop_watch_start(&_sw.watch), 1): 0; \
        stop_watch_stop(&_sw.watch), \
        __stop_watch_wrapper_time_diff(&_sw.watch, &_sw.diff), \
        printf("diff_sec: %ld | diff_nsec: %ld\n", _sw.diff.tv_sec, _sw.diff.tv_nsec), \
        _sw.run = 0 \
    )
    
#endif
