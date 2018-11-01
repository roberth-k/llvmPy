; INPUT: llvmPy.ir -c "foo = None; foo()"

%PyObj = type opaque
%FrameN = type opaque
%Frame1 = type <{ %Frame1*, %FrameN*, [1 x %PyObj*] }>

define %PyObj* @__body__(%FrameN* %outer) prefix i64 //[0-9]+// {
  %frame = alloca %Frame1
  %1 = getelementptr %Frame1, %Frame1* %frame, i64 0, i32 0
  store %Frame1* %frame, %Frame1** %1
  %2 = getelementptr %Frame1, %Frame1* %frame, i64 0, i32 1
  store %FrameN* %outer, %FrameN** %2
  %3 = getelementptr %Frame1, %Frame1* %frame, i64 0, i32 2, i64 0
  store %PyObj* null, %PyObj** %3

  %4 = call %PyObj* @llvmPy_none()
  store %PyObj* %4, %PyObj** %3

  %5 = load %PyObj*, %PyObj** %3
  %callframe = alloca %FrameN*
  %6 = call %PyObj* ()* @llvmPy_fchk(%FrameN** %callframe, %PyObj* %5, i64 0)
  %7 = call %PyObj* %6(%FrameN** %callframe)

  ret %PyObj* null
}

declare %PyObj* @llvmPy_none()
declare %PyObj* ()* @llvmPy_fchk(%FrameN**, %PyObj*, i64)