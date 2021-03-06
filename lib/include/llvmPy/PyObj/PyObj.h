#pragma once

#include <llvmPy/Typed.h>
#include <string>

#ifdef __cplusplus
namespace llvmPy {

/**
 * The base class for all objects accessible on the heap at runtime.
 */
class PyObj : public Typed {
public:
    PyObj();

    PyObj(PyObj const &copy) noexcept;

    PyObj(PyObj &&move) noexcept;

    virtual ~PyObj();

    virtual std::string py__str__();

    virtual bool py__bool__();

    virtual int64_t py__int__();

    virtual int64_t py__len__();

    virtual PyObj * py__add__(PyObj &rhs);
    virtual PyObj * py__sub__(PyObj &rhs);
    virtual PyObj * py__mul__(PyObj &rhs);

    virtual bool py__lt__(PyObj &rhs);
    virtual bool py__le__(PyObj &rhs);
    virtual bool py__eq__(PyObj &rhs);
    virtual bool py__ne__(PyObj &rhs);
    virtual bool py__ge__(PyObj &rhs);
    virtual bool py__gt__(PyObj &rhs);

    virtual PyObj *py__getattr__(std::string const &name);

    virtual PyObj *py__getitem__(PyObj &key);

    /** Return true if this is an instance of a class, i.e. method calls
     *  should automatically bind the self-argument.
     */
    virtual bool isInstance() const;

protected:
    virtual int compare(PyObj &rhs) const;
};

} // namespace llvmPy
#endif // __cplusplus
