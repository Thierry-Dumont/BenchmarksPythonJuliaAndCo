from time import perf_counter as time
import socket

import numpy as np

from rando import Rando
from stiffness import Stiffness
from random_triangle import random_triangle

try:
    from time import perf_counter_ns as time
except ImportError:
    pass


x = np.empty(3)
y = np.empty(3)
mat = np.empty(21)
stiffness = Stiffness()

print("\nVerify that, on the reference element, we are coherent with sage")
print("(see ../sage/):\n")
x[0] = 0.0
x[1] = 1.0
x[2] = 0.0
y[0] = 0.0
y[1] = 0.0
y[2] = 1.0
stiffness.op(x, y, mat)

for i in range(6):
    print([mat[i * (i + 1) // 2 + j] for j in range(0, i + 1)])
print("\nWe must get the same result if we dilate the triangle:\n")
for i in range(3):
    x[i] *= 2.0
    y[i] *= 2
stiffness.op(x, y, mat)
for i in range(6):
    print([mat[i * (i + 1) // 2 + j] for j in range(0, i + 1)])

print("\nNow, start the benchmark:")
ntri = 1_000_000
random_generator = Rando()

times = np.empty(ntri)

for tr in range(ntri):
    random_triangle(random_generator, x, y)
    t1 = time()
    stiffness.op(x, y, mat)
    times[tr] = time() - t1

if time.__name__.endswith("_ns"):
    times *= 1e-9

print(f"Time by triangle: {np.median(times) * 1e6:.2f} µs. (median)")
print(f"Time by triangle: {np.min(times) * 1e6:.2f} µs. (min)")

from transonic.util import timeit

time_timeit = timeit("stiffness.op(x, y, mat)", globals=locals())

print(f"Time by triangle: {time_timeit * 1e6:.2f} µs. (timeit)")

with open("RunningOn" + socket.gethostname(), "w") as file:
    file.write(f"{time_timeit}\n")
