#ifndef __WHEEL_FACTORIZE
#define __WHEEL_FACTORIZE

#include <stdbool.h>

struct wheel_factor {
    long *basis;
    long basis_size;
    long *wheel;
    long wheel_size;
};

int generate_basis(struct wheel_factor *wf, int basis_size);
int generate_wheel(struct wheel_factor *wf);
long* factors(struct wheel_factor *wf, long n, long *count);
bool is_prime(long n);

#endif /* __WHEEL_FACTORIZE */