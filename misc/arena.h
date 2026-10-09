#ifndef ARENA
#define ARENA

#include "error.h"

/*
 * this make use of the mem page in Window and Linux
 * by preallocate a specific amount of page table (and allocate more if need more)
 * continous mem (data locality) offer maximum performance (maximize TBL hit)
 * a single drawback is that once it done its job, the whole thing (arena) must be free
 */

typedef struct{
};

#endif