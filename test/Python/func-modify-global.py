# RUN: llvmPy %s > %t1
# RUN: cat -n %t1 >&2
# RUN: cat %t1 | FileCheck %s

y = 1
print(y)
# CHECK: 1

def func():
    y = 2

func()
print(y)
# CHECK: 1

# TODO: `y` isn't marked as "global" (not supported); hence the behaviour.
