#ifndef MISC_H
#define MISC_H

#define AS(type, value) ((type)value)
#define VA(type, value) (*(type) value)

int max(int a, int b)
{
    return (a > b)? a : b;
}
int larger(int a, int b)
{
    return (a > b)? 0 : 1;
}

#endif