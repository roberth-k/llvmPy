#pragma once

#include <stdint.h>

#ifdef __cplusplus

namespace llvm {
class DataLayout;
class IntegerType;
class LLVMContext;
class FunctionType;
class PointerType;
class StructType;
class Type;
} // namespace llvm

namespace llvmPy {

class PyObj;
class PyInt;

class Types {
public:
    Types(llvm::LLVMContext &, llvm::DataLayout const &);

    llvm::StructType *PyObj; ///< Opaque structure type.
    llvm::PointerType *Ptr; ///< Pointer to PyObj.

    llvm::IntegerType *PyIntValue;

    llvm::FunctionType *llvmPy_add;
    llvm::FunctionType *llvmPy_int;
};

}

extern "C" {

llvmPy::PyObj *llvmPy_add(llvmPy::PyObj &, llvmPy::PyObj &);
llvmPy::PyInt *llvmPy_int(int64_t value);

} // extern "C"

#endif // __cplusplus