#pragma once
#include <unordered_map>
#include <string>
#include <llvmPy/RT/Frame.h>
#include <llvmPy/RT/Scope.h>
#include <llvmPy/Support/Testing.h>

#ifdef __cplusplus
namespace llvm {
class Function;
class LLVMContext;
class Module;
class Value;
class GlobalVariable;
class StructType;
} // namespace llvm

namespace llvmPy {

class Compiler;
class Types;
class RTModule;
class PyObj;

class RTScope : public Scope {
public:
    struct Slot {
        llvm::Value *value;
        size_t frameIndex;
    };

    explicit RTScope(RTScope &parent);
    explicit RTScope(RTModule &module);

public:
    RTScope *createDerived();

public:
    RTModule &getModule() const;

    bool hasSlot(std::string const &name) const;

    void declareSlot(std::string const &name);

    llvm::Value *getSlotValue(std::string const &name) const;

    void setSlotValue(std::string const &name, llvm::Value *value);

    size_t getSlotIndex(std::string const &name) const;

    size_t getSlotCount() const override;

    llvm::StructType *getInnerFrameType() const;

    void setInnerFrameType(llvm::StructType *st);

    llvm::StructType *getOuterFrameType() const;

    void setOuterFrameType(llvm::StructType *st);

    llvm::Value *getInnerFramePtrPtr() const;

    void setInnerFramePtrPtr(llvm::Value *ptr);

    llvm::Value *getInnerFramePtr() const;

    void setInnerFramePtr(llvm::Value *ptr);

    size_t getNextCondStmtIndex();

    size_t getNextWhileStmtIndex();

    void setCallFramePtr(llvm::Value *ptr);

    llvm::Value *getCallFramePtr() const;

private:
    RTModule &_module;
    llvm::Value *_innerFramePtr;
    llvm::Value *_innerFramePtrPtr;
    llvm::StructType *_innerFrameType;
    llvm::StructType *_outerFrameType;
    size_t _condStmtCount;
    size_t _whileStmtCount;
    std::unordered_map<std::string, Slot> _slots;
    llvm::Value *_callframePtr;
};

} // namespace llvmPy
#endif // __cplusplus

