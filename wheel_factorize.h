#ifndef __WHEEL_FACTORIZE
#define __WHEEL_FACTORIZE

#include <stdbool.h>
#include <malloc.h>

struct wheel_factor {
    long *basis;
    long basis_size;
    long *wheel;
    long wheel_size;
    long *increments;
};

int wheel_factor_init(struct wheel_factor *wf, int basis_size);
long* factors(struct wheel_factor *wf, long n, long *count);
bool is_prime(struct wheel_factor *wf, long n);

#endif /* __WHEEL_FACTORIZE */
