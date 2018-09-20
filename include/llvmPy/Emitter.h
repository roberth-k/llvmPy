#pragma once
#include <llvmPy/AST.h>
#include <llvmPy/Compiler.h>
#include <llvmPy/Instr.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include "llvm/IR/NoFolder.h"

#ifdef __cplusplus
namespace llvm {
class DataLayout;
class IntegerType;
class Module;
class Value;
} // namespace llvm

namespace llvmPy {

class RT;
class RTAtom;
class RTScope;
class RTFuncObj;
class RTModuleObj;

class Emitter {
public:
    explicit Emitter(Compiler &c) noexcept;

    RTModuleObj *
    emitModule(
            std::vector<Stmt *> const &stmts,
            std::string const &name);

    RTFuncObj *
    emitFunction(
            std::string const &name,
            std::vector<std::string const *> args,
            std::vector<Stmt *> body,
            llvm::Module &module,
            RTScope *scope);

private:
    RT &rt;
    llvm::DataLayout const &dl;
    llvm::LLVMContext &ctx;
    llvm::IRBuilder<> ir;
    Types types;

    llvm::Value * emit(
            AST const &ast,
            llvm::Module &module,
            RTScope &outer);

    llvm::AllocaInst * emitAlloca(RTAtom const &);
    llvm::AllocaInst * emitAlloca(RTAtom const &, std::string const &);
};

} // namespace llvmPy
#endif // __cplusplus
