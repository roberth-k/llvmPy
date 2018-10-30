#pragma once

#include <stdint.h>
#include <llvmPy/PyObj.h>

#ifdef __cplusplus

namespace llvm {
class DataLayout;
class IntegerType;
class LLVMContext;
class Function;
class FunctionType;
class PointerType;
class StructType;
class Type;
} // namespace llvm

namespace llvmPy {

class Types {
public:
    Types(llvm::LLVMContext &, llvm::DataLayout const &);

    llvm::StructType *PyObj; ///< Opaque structure type.
    llvm::PointerType *Ptr; ///< Pointer to PyObj.
    llvm::FunctionType *Func; ///< Opaque function.
    llvm::PointerType *FuncPtr; ///< Pointer to opaque function.
    llvm::StructType *FrameN; ///< Frame of unknown size (opaque).
    llvm::PointerType *FrameNPtr;
    llvm::PointerType *FrameNPtrPtr;

    llvm::StructType *getFrameN() const;
    llvm::StructType *getFrameN(int N) const;

    llvm::IntegerType *PyIntValue;

    llvm::FunctionType *llvmPy_add;
    llvm::FunctionType *llvmPy_int;
    llvm::FunctionType *llvmPy_none;
    llvm::FunctionType *llvmPy_func;
    llvm::FunctionType *llvmPy_fchk;

private:
    llvm::LLVMContext &ctx;
};

template<int N>
struct Frame {
    Frame<N> *self;
    Frame<0> *outer;
    PyObj *vars[N];
};

typedef Frame<0> FrameN;

}

extern "C" {

llvmPy::PyObj *llvmPy_add(llvmPy::PyObj &, llvmPy::PyObj &);
llvmPy::PyInt *llvmPy_int(int64_t value);
llvmPy::PyNone *llvmPy_none();
llvmPy::PyFunc *llvmPy_func(llvmPy::FrameN *frame, llvm::Function *function);
llvm::Function *llvmPy_fchk(llvmPy::FrameN **callframe, llvmPy::PyFunc &func, int np);

} // extern "C"

#endif // __cplusplus
