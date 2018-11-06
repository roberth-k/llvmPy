#include <llvmPy/Compiler.h>
#include <llvmPy/PyObj.h>
#include "CompilerImpl.h"
#include <iostream>
#include <stdlib.h>
using namespace llvmPy;
using std::cerr;
using std::endl;

Compiler::Compiler() noexcept
: impl(std::make_unique<CompilerImpl>())
{
}

Compiler::~Compiler() = default;

llvm::DataLayout const &
Compiler::getDataLayout() const
{
    return impl->getDataLayout();
}

void
Compiler::addAndRunModule(std::unique_ptr<llvm::Module> module)
{
    impl->addModule(std::move(module));
    llvm::JITSymbol moduleBody = impl->findSymbol("__body__");

    auto expectedTargetAddress = moduleBody.getAddress();

    if (auto error = expectedTargetAddress.takeError()) {
        llvm::logAllUnhandledErrors(std::move(error), llvm::errs(), "Error: ");
        exit(2);
    }

    llvm::JITTargetAddress targetAddress = expectedTargetAddress.get();
    auto moduleBodyPtr = reinterpret_cast<void(*)(void *)>(targetAddress);
    moduleBodyPtr(nullptr);
}
