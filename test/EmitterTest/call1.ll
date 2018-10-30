; INPUT: llvmPy.ir -c "foo = None; foo(1)"

%PyObj = type opaque

define %PyObj* @__body__() prefix i64 //[0-9]+// {
  %1 = alloca %PyObj*
  store %PyObj* null, %PyObj** %1
  %2 = call %PyObj* @llvmPy_none()
  store %PyObj* %2, %PyObj** %1
  %fo = load %PyObj*, %PyObj** %1
  %a = call %PyObj* @llvmPy_int(i64 1)
  %fp = call %PyObj* ()* @llvmPy_fchk(%PyObj* %fo, i64 1)
  %rv = call %PyObj* %fp(%PyObj* %a)
  ret %PyObj* null
}

declare %PyObj* @llvmPy_none()
declare %PyObj* @llvmPy_int(i64)
declare %PyObj* ()* @llvmPy_fchk(%PyObj*, i64)
