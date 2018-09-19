#include <sys/time.h>
#include <sys/resource.h>

#include "wheel_factorize.h"

double get_time() {
    struct timeval t;
    struct timezone tzp;
    gettimeofday(&t, &tzp);
    return t.tv_sec*1e6 + t.tv_usec;
}

double benchmark(long n, long reps) {
    struct wheel_factor wf;
    wheel_factor_init(&wf, 3);

    long count;
    long *primes;
    double start_time, end_time;
    double sum = 0;

    // measure the factors function 'reps' times
    for (int i = 0; i < reps; i++) {
        start_time = get_time();
        primes = factors(&wf, n, &count);
        end_time = get_time();

        free(primes);
        sum += (end_time - start_time);
    }

    return sum / reps;
}

int main(int argc, char **argv) {
    for (long i = 1; i <= 1000000; i++) {
        printf("%ld,%f\n", i, benchmark(i, 10000));
    }
    return 0;
}
