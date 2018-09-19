#include <malloc.h>
#include <stdlib.h>
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
    // test whether it performs better
    int *a = malloc(n * sizeof(int));
    if (a == NULL) {
        return NULL;
    }
    memset(a, 1, n * sizeof(int));
    long max = (long) ceil(sqrt(n));

    for (long i = 2; i < max; i++) {
        if (a[i]) {
            for (long j = i*i; j < n; j += i) {
                a[j] = 0;
            }
        }
    }

    long count = 0;
    for (long i = 2; i < n; i++) {
        if (a[i]) {
            count++;
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

    // generate increment array (increments between wheel elements)
    long *inc = malloc(wf->wheel_size * sizeof(long));
    if (inc == NULL) {
        return 2;
    }
    for (int i = 0; i < wf->wheel_size-1; i++) {
        inc[i] = wf->wheel[i+1] - wf->wheel[i];
    }
    // wrap last element
    inc[wf->wheel_size-1] = wf->wheel[0]
        + prod_arr(wf->basis, wf->basis_size)
        - wf->wheel[wf->wheel_size-1];
    wf->increments = inc;

    return 0;
}
long* factors(struct wheel_factor *wf, long n, long *count) {
    if (n <= 0) {
        return NULL;
    }
    // create large enough factors array
    // TODO: start small and increase as needed
    long *factors = malloc(((long) sqrt(n) + 2) * sizeof(long));
    if (factors == NULL) {
        return NULL;
    }
    long index = 0;
    factors[index++] = 1;

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
            k += wf->increments[i];
            i = (i+1) % wf->wheel_size;
        }
    }
    if (n != 1) {
        factors[index++] = n;
    }

    *count = index;
    return factors;
}
bool is_prime(struct wheel_factor *wf, long n) {
    // modified factors function that returns
    // as soon as more than 1 factor is found
    if (n <= 1) {
        return false;
    }
    long factor_count = 0;

    // add factors from basis
    for (int i = 0; i < wf->basis_size; i++) {
        long v = wf->basis[i];
        while (n % v == 0) {
            if (factor_count++ >= 1)
                return false;
            n /= v;
        }
    }

    // add factors from wheel
    int i = 0;
    long k = wf->wheel[0];
    while (k*k <= n) {
        if (n % k == 0) {
            if (factor_count++ >= 1)
                return false;
            n /= k;
        }
        else {
            k += wf->increments[i];
            i = (i+1) % wf->wheel_size;
        }
    }
    if (n != 1) {
        if (factor_count++ >= 1)
            return false;
    }

    return true;
}
int wheel_factor_init(struct wheel_factor *wf, int basis_size) {
    int ret;
    if (ret = generate_basis(wf, basis_size)) {
        return ret;
    }
    if (ret = generate_wheel(wf)) {
        return ret | 0x10;
    }
    return 0;
}
