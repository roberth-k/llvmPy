#pragma once

#include "llvmPy/Pass.h"
#include <string>

#ifdef __cplusplus
namespace llvmPy {

class FuncTree;
class ModuleTree;
class ScopeTree;
class Stmt;

struct AnalysisPassI {
    std::string moduleName;
    std::shared_ptr<Stmt const> stmt;
};

struct AnalysisPassO {
    std::shared_ptr<ModuleTree const> moduleTree;
};

class AnalysisPass : public Pass<AnalysisPassI, AnalysisPassO> {
public:

    AnalysisPassO run(AnalysisPassI const &input) override;

private:

    /** Declare slots inferred from the function's signature. */
    void initScopeSlotsFromSignature(ScopeTree &st, FuncTree const &ft) const;

    /** Declare slots inferred from the function's statements. */
    void initScopeSlotsFromBody(ScopeTree &st, Stmt const &stmt) const;

    /** Set the properties of the scope's frames based on scope declarations
     *  acquired from other methods. */
    void initScopeFrames(ScopeTree &st, FuncTree const &ft) const;
};

} // namespace llvmPy
#endif // __cplusplus