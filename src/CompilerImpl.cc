#include "CompilerImpl.h"
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/IR/Mangler.h>
#include <algorithm>
#include <sstream>
using namespace llvmPy;

static std::unique_ptr<llvm::TargetMachine>
initTargetMachine() {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmParser();
    llvm::InitializeNativeTargetAsmPrinter();
    return std::make_unique<llvm::TargetMachine>(
            llvm::EngineBuilder().selectTarget());
}

CompilerImpl::CompilerImpl()
        : targetMachine(initTargetMachine()),
          dataLayout(targetMachine->createDataLayout()),
          symbolResolver(createSymbolResolver()),
          objectLayer(executionSession, createResourcesGetter()),
          compileLayer(objectLayer, llvm::orc::SimpleCompiler(*targetMachine))
{
    // TODO: What does this accomplish?
    llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
}

llvm::orc::VModuleKey
CompilerImpl::addModule(std::unique_ptr<llvm::Module> module)
{
    llvm::orc::VModuleKey moduleKey = executionSession.allocateVModule();
    llvm::cantFail(compileLayer.addModule(moduleKey, std::move(module)));
    moduleKeys.push_back(moduleKey);
    return moduleKey;
}

llvm::JITSymbol
CompilerImpl::findSymbol(std::string const &name)
{
    std::string const mangledName = mangleSymbol(name);

    // Search keys in reverse order.
    for (auto i = moduleKeys.size() - 1; i >= 0; i--){
        auto moduleKey = moduleKeys[i];
        auto symbol = compileLayer.findSymbolIn(moduleKey, mangledName, true);

        if (symbol) {
            return symbol;
        }
    }

    return nullptr;
}

std::shared_ptr<llvm::orc::SymbolResolver>
CompilerImpl::createSymbolResolver()
{
    return llvm::orc::createLegacyLookupResolver(
            executionSession,
            [this](std::string const &name) {
                return objectLayer.findSymbol(name, true);
            },
            [](llvm::Error err) {
                llvm::cantFail(
                        std::move(err),
                        "lookup failed");
            });
}

TObjectLayer::ResourcesGetter
CompilerImpl::createResourcesGetter() const
{
    return [this](llvm::orc::VModuleKey moduleKey) {
        return TObjectLayer::Resources {
                std::make_shared<llvm::SectionMemoryManager>(),
                symbolResolver,
        };
    };
}

std::string
CompilerImpl::mangleSymbol(std::string const &symbol) const
{
    std::string s;
    llvm::raw_string_ostream ss(s);
    llvm::Mangler::getNameWithPrefix(ss, symbol, dataLayout);
    return ss.str();
}
