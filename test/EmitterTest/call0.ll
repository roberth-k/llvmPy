; INPUT: llvmPy.ir --naked -c "foo = None; foo()"

%PyObj = type opaque

define void @__body__() {
  %1 = alloca %PyObj*
  store %PyObj** null, %PyObj** %1
  %2 = call %PyObj* @llvmPy_none()
  store %PyObj* %2, %PyObj** %1
  %fo = load %PyObj*, %PyObj** %1
  %fp = call %PyObj* ()* @llvmPy_fchk(%PyObj* %fo, i64 0)
  %rv = call %PyObj* %fp()
}

declare %PyObj* @llvmPy_none()
declare %PyObj* ()* @llvmPy_fchk(%PyObj*, i64)
