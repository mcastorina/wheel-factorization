import sys
from wheel_factorize import WheelFactor

if __name__ == '__main__':
    wf = WheelFactor(3)
    for arg in sys.argv[1:]:
        print("%s: %s" % (arg, str(wf.factors(int(arg)))))
