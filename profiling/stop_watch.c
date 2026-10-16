#include "stop_watch.h"


// for loop (init; condition; update)

void stop_watch_start(StopWatch* watch)
{
    clock_gettime(CLOCK_MONOTONIC, &watch->start);
    // clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &watch->start);
}
void stop_watch_stop(StopWatch* watch)
{
    clock_gettime(CLOCK_MONOTONIC, &watch->stop);
}

timespec time_diff(StopWatch* watch)
{
    timespec diff;
    timespec start = watch->start;
    timespec stop = watch->stop;

    // 3.456 -> 6.123 = 2.667
    if ((stop.tv_nsec - start.tv_nsec) < 0)
    {
        // sec interval minus 1 sec
        // nano sec interval adds 1 sec
        diff.tv_sec = (stop.tv_sec - start.tv_sec) - 1;
        diff.tv_nsec = (1000000000 + stop.tv_nsec) - start.tv_nsec;
    }
    else
    {
        diff.tv_sec = stop.tv_sec - start.tv_sec;
        diff.tv_nsec = stop.tv_nsec - start.tv_nsec;
    }
    return diff;
}

