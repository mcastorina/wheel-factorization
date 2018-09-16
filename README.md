# wheel-factorization

Wheel factorization is an improvement of trial division method for
integer factorization. This project implements wheel factorization in
Python and C.

[Wikipedia Link](https://en.wikipedia.org/wiki/Wheel_factorization)

## Usage

#### Python

```python
from wheel_factorize import WheelFactor as WF
wf = WF(3)                  # basis size
print(wf.factors(10))
print(wf.factors(31))
print(wf.factors(9999991))
print(wf.factors(22222222222))
```

#### C

```C
#include "wheel_factorize.h"

struct wheel_factor wf;
generate_basis(&wf, 3);     // basis size
generate_wheel(&wf);

long count;
long *primes = factors(&wf, 10, &count);            free(primes);
      primes = factors(&wf, 31, &count);            free(primes);
      primes = factors(&wf, 9999991, &count);       free(primes);
      primes = factors(&wf, 22222222222, &count);   free(primes);
```

Example

```bash
$ make
$ ./examples/example 10 31 9999991 22222222222
```

## Takeaways
I learned a lot of cool math doing this simple project, namely the
[Sieve of Eratosthenes](https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes),
an ancient algorithm for finding all the primes up to a given n.
