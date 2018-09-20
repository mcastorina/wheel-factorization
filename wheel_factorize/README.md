# wheel_factorize

Wheel factorization is an improvement of trial division method for
integer factorization. This project is the Python implementation of
wheel factorization.

[Wikipedia Link](https://en.wikipedia.org/wiki/Wheel_factorization)

[Source](https://github.com/mcastorina/wheel-factorization)

## Usage

```python
from wheel_factorize import WheelFactor as WF

wf = WF(3)                  # basis size
print(wf.factors(10))
print(wf.factors(31))
print(wf.factors(22222222222))
print(wf.is_prime(9999991))
```
