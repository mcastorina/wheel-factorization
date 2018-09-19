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
        inc = [self.wheel[i] - self.wheel[i-1] for i in range(1, len(self.wheel))]
        inc += [self.wheel[0] + np.prod(self.basis) - self.wheel[-1]]
        self.increments = inc
    def factors(self, n):
        if n <= 0: return None
        _factors = [1]
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
                k += self.increments[i]
                i = (i+1) % len(self.increments)
        if n != 1: _factors += [n]
        return _factors
    def is_prime(self, n):
        # modified wheel that will quit early if 2 factors found
        if n <= 1: return False
        factor_count = 0
        for v in self.basis:
            while n % v == 0:
                if factor_count >= 1: return False
                factor_count += 1
                n //= v
        i = 0
        k = self.wheel[0]
        while k*k <= n:
            if n % k == 0:
                if factor_count >= 1: return False
                factor_count += 1
                n //= k
            else:
                k += self.increments[i]
                i = (i+1) % len(self.increments)
        if n != 1:
            if factor_count >= 1: return False
            factor_count += 1
        return True
