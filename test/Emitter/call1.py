# RUN: llvmPy --ir %s > %t1
# RUN: cat -n %t1 >&2
# RUN: llvm-as < %t1 | llvm-dis | FileCheck %s

f = None
f(1)

# CHECK-LABEL: define %PyObj* @__body__
# CHECK: [[_None:%[0-9]+]] = call %PyObj* @llvmPy_none()
# CHECK-NEXT: store %PyObj* [[_None]], %PyObj** [[_None2:%[a-z_0-9]+]]
# CHECK: [[None:%[0-9]+]] = load %PyObj*, %PyObj** [[_None2]]
# CHECK: [[_1:%[0-9]+]] = call %PyObj* @llvmPy_int(i64 1)
# CHECK: %callframe = alloca %FrameN*
# CHECK: [[_FUNC:%[0-9]+]] = call i8* @llvmPy_fchk(%FrameN** %callframe, %PyObj* [[None]], i64 1)
# CHECK-NEXT: [[FUNC:%[0-9]+]] = bitcast i8* [[_FUNC]] to %PyObj* (%FrameN**, %PyObj*)
# CHECK-NEXT: {{%[0-9]+}} = call %PyObj* [[FUNC]](%FrameN** %callframe, %PyObj* [[_1]])

# CHECK-DAG: declare i8* @llvmPy_fchk(%FrameN**, %PyObj*, i64)
