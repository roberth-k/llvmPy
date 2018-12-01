#pragma once

#include <string>

#ifdef __cplusplus
namespace llvmPy {

class FuncTree;
class ScopeTree;

class ModuleTree {
public:
    std::string const &getName() const;

    void setName(std::string const &name);

    FuncTree &getFuncTree() const;

    ScopeTree &getScopeTree() const;

private:

    std::string _name;

    FuncTree *_funcTree;
};

} // namespace llvmPy
#endif // __cplusplus