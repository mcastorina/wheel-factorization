import numpy as np

# the Sieve of Eratosthenes
# generates a list of primes < n
def sieve_of_eratosthenes(n):
    a = np.ones(n)
    for i in range(2, int(np.ceil(np.sqrt(n)))):
        if a[i]:
            for j in range(i*i, n, i):
                a[j] = 0
    return [i for i in range(2, len(a)) if a[i]]

def is_prime(n):
    # modified sieve that will quit early if multiple found
    if n < 2: return False
    n += 1
    a = np.ones(n)
    for i in range(2, int(np.ceil(np.sqrt(n)))):
        if a[i]:
            if (n-1) % i == 0: return False
            for j in range(i*i, n, i):
                a[j] = 0
    return True

def coprime(a, b):
    return np.gcd(a, b) == 1

class WheelFactor:
    def __init__(self, basis_size=3, basis=None):
        # get basis
        if basis: self.basis = basis
        else:     self.generate_basis(basis_size)
        # get first turn
        self.generate_wheel()

    def generate_basis(self, basis_size):
        self.basis = sieve_of_eratosthenes(basis_size**2 + 1)[:basis_size]
        assert(len(self.basis) == basis_size)
    def generate_wheel(self):
        # generate wheel
        self.wheel = []
        for n in range(max(self.basis) + 1, np.prod(self.basis) + 2):
            if all(map(lambda y: coprime(y, n), self.basis)):
                self.wheel.append(n)
    def factors(self, n):
        if n <= 0: return None
        _factors = [1]
        inc = [self.wheel[i] - self.wheel[i-1] for i in range(1, len(self.wheel))]
        for v in self.basis:
            while n % v == 0:
                _factors += [v]
                n //= v
        i = 0
        k = self.wheel[0]
        while k*k <= n:
            if n % k == 0:
                _factors += [k]
                n //= k
            else:
                k += inc[i]
                i = (i+1) % len(inc)
        if n != 1: _factors += [n]
        return _factors
