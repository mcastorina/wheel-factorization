#include <stdio.h>
#include <stdlib.h>

#include "wheel_factorize.h"

void print_factors(struct wheel_factor *wf, long n) {
    long count;
    long *primes = factors(wf, n, &count);
    if (primes == NULL) {
        printf("%ld: ERROR\n", n);
        return;
    }
    printf("%ld: [", n);                // prefix
    for (int i = 0; i < count-1; i++) {
        printf("%ld, ", primes[i]);     // body
    }
    printf("%ld]\n", primes[count-1]);  // postfix
}
int main(int argc, char **argv) {
    // TODO: add flag for basis size
    // initialize struct
    struct wheel_factor wf;
    wheel_factor_init(&wf, 3);

    for (int i = 1; i < argc; i++) {
        long input = strtol(argv[i], NULL, 10);
        print_factors(&wf, input);
    }
    return 0;
}
