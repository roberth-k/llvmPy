# RUN: test-ir.sh %s
# RUN: test-output.sh %s

# Check that the global isn't defined multiple times.
# The `print()` is necessary, as otherwise the value would be discarded.

# IR: @llvmPy_False = external constant %PyObj
# IR-NOT: @llvmPy_False
# IR-LABEL: define %PyObj* @__body__
print(False)
# IR: {{%[0-9]+}} = call %PyObj* @llvmPy_print(%PyObj* @llvmPy_False)
print(False)
# IR-NEXT: {{%[0-9]+}} = call %PyObj* @llvmPy_print(%PyObj* @llvmPy_False)

# OUTPUT: False
# OUTPUT-NEXT: False
