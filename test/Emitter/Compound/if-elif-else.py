# RUN: %S/../test.sh %s


# CHECK-LABEL: define %PyObj* @__body__

x = 0

# CHECK: [[RESULT:%[a-z_0-9]+]] = call %PyObj* @llvmPy_eq
# CHECK-NEXT: [[TRUTHY:%[a-z_0-9]+]] = call i1 @llvmPy_truthy(%PyObj* [[RESULT]])
# CHECK-NEXT: br i1 [[TRUTHY]], label %then, label %else
# CHECK-LABEL: then:
if x == 1:
    x = 10
# CHECK-LABEL: else:
# CHECK-LABEL: endif:
# CHECK: [[RESULT1:%[a-z_0-9]+]] = call %PyObj* @llvmPy_ge
# CHECK-NEXT: [[TRUTHY1:%[a-z_0-9]+]] = call i1 @llvmPy_truthy(%PyObj* [[RESULT1]])
# CHECK-NEXT: br i1 [[TRUTHY1]], label %then1, label %else1
# CHECK-LABEL: then1:
elif x >= 2:
    x = 11
# CHECK-LABEL: else1:
# CHECK: [[RESULT2:%[a-z_0-9]+]] = call %PyObj* @llvmPy_ne
# CHECK-NEXT: [[TRUTHY2:%[a-z_0-9]+]] = call i1 @llvmPy_truthy(%PyObj* [[RESULT2]])
# CHECK-NEXT: br i1 [[TRUTHY2]], label %then2, label %else2
elif x != 3:
    # CHECK-LABEL: then2:
    x = 12
# CHECK-LABEL: else2:
else:
    x = 13

print(x)

# CHECK-LABEL: endif2
# CHECK: @llvmPy_print(
# CHECK-NEXT: ret %PyObj* @llvmPy_None
