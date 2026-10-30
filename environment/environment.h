#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

// #include "../misc/"

// HashStringToIns();
// ownership?

struct Environment { 
    struct Environment* parent;
};
typedef struct Environment Environment;


#endif