#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "wheel_factorize.h"

/* helper functions */

// the Sieve of Eratosthenes
// generates a list of primes < n
long* sieve_of_eratosthenes(long n, long *length) {
    // TODO: we can save space by using a bitmap
    uint8_t *a = malloc(n * sizeof(uint8_t));
    if (a == NULL) {
        return NULL;
    }
    memset(a, 1, n * sizeof(uint8_t));
    long max = (long) ceil(sqrt(n));

    long count = n-2;   // keep track of the number of
                        // true values in 'a'
    for (long i = 2; i < max; i++) {
        if (a[i]) {
            for (long j = i*i; j < n; j += i) {
                count -= a[j];  // sometimes a[j] is cleared multiple times
                a[j] = 0;       // might be better to count at the end
                                // to avoid read + write
            }
        }
    }

    long *result = malloc(count * sizeof(long));
    if (result == NULL) {
        free(a);
        return NULL;
    }

    // build array of prime numbers
    long index = 0;
    for (long i = 2; i < n; i++) {
        if (a[i]) {
            result[index++] = i;
        }
    }

    free(a);
    *length = count;
    return result;
}
bool is_prime(long n) {
    // modified sieve that will quit early if multiple found
    if (n < 2) {
        return false;
    }
    n += 1;             // add 1 because the sieve finds primes < n
    bool result = true; // true until proven false

    // TODO: we can save space by using a bitmap
    uint8_t *a = malloc(n * sizeof(uint8_t));
    if (a == NULL) {
        // TODO: notify caller of error
        return NULL;
    }
    memset(a, 1, n * sizeof(uint8_t));

    long max = (long) ceil(sqrt(n));
    for (long i = 2; i < max; i++) {
        if (a[i]) {
            // if i is a divisor of n-1 (because we added 1 at the start)
            if ((n-1) % i == 0) {
                result = false;
                goto is_prime_done;
            }
            // clear future entries
            for (long j = i*i; j < n; j += i) {
                a[j] = 0;
            }
        }
    }

is_prime_done:
    free(a);
    return result;
}
long gcd(long a, long b) {
    long tmp;
    while (b != 0) {
        tmp = a % b;

        a = b;
        b = tmp;
    }
    return a;
}
bool coprime(long a, long b) {
    return gcd(a, b) == 1;
}
bool all_coprime(long n, long *arr, long arr_len) {
    for (long i = 0; i < arr_len; i++) {
        if (!coprime(n, arr[i]))
            return false;
    }
    return true;
}
long prod_arr(long *arr, long arr_len) {
    // TODO: detect overflow
    long prod = 1;
    for (long i = 0; i < arr_len; i++) {
        prod *= arr[i];
    }
    return prod;
}

/* struct wheel_factor functions */
int generate_basis(struct wheel_factor *wf, int basis_size) {
    long count;
    long *primes = sieve_of_eratosthenes(basis_size * basis_size, &count);
    if (primes == NULL) {
        return 1;
    }
    long *result = malloc(basis_size * sizeof(long));
    if (result == NULL) {
        free(primes);
        return 1;
    }

    memcpy(result, primes, basis_size * sizeof(long));
    free(primes);

    wf->basis = result;
    wf->basis_size = basis_size;
    return 0;
}
int generate_wheel(struct wheel_factor *wf) {
    // calculate product of the basis
    long prod = prod_arr(wf->basis, wf->basis_size);

    // TODO: wasting space
    long *wheel = malloc((prod + 2 - wf->basis[wf->basis_size-1]) * sizeof(long));
    if (wheel == NULL) {
        return 1;
    }

    long index = 0;
    for (int n = wf->basis[wf->basis_size-1]; n < prod + 2; n++) {
        if (all_coprime(n, wf->basis, wf->basis_size)) {
            wheel[index++] = n;
        }
    }

    wf->wheel = wheel;
    wf->wheel_size = index;
    return 0;
}
long* factors(struct wheel_factor *wf, long n, long *count) {
    if (n <= 0) {
        return NULL;
    }
    // create large enough factors array
    // TODO: start small and increase as needed
    long *factors = malloc(((long) sqrt(n)) * sizeof(long));
    if (factors == NULL) {
        return NULL;
    }
    long index = 0;
    factors[index++] = 1;

    // generate inc array (increments between wheel elements)
    long inc[wf->wheel_size];
    for (int i = 0; i < wf->wheel_size-1; i++) {
        inc[i] = wf->wheel[i+1] - wf->wheel[i];
    }
    // wrap last element
    inc[wf->wheel_size-1] = wf->wheel[0]
        + prod_arr(wf->basis, wf->basis_size)
        - wf->wheel[wf->wheel_size-1];

    // add factors from basis
    for (int i = 0; i < wf->basis_size; i++) {
        long v = wf->basis[i];
        while (n % v == 0) {
            factors[index++] = v;
            n /= v;
        }
    }

    // add factors from wheel
    int i = 0;
    long k = wf->wheel[0];
    while (k*k <= n) {
        if (n % k == 0) {
            factors[index++] = k;
            n /= k;
        }
        else {
            k += inc[i];
            i = (i+1) % wf->wheel_size;
        }
    }
    if (n != 1) {
        factors[index++] = n;
    }

    *count = index;
    return factors;
}

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
    // build struct
    struct wheel_factor wf;
    generate_basis(&wf, 3);
    generate_wheel(&wf);

    for (int i = 1; i < argc; i++) {
        long input = strtol(argv[i], NULL, 10);
        print_factors(&wf, input);
    }
    return 0;
}
