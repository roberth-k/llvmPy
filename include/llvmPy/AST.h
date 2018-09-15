#pragma once
#include <llvmPy/Token.h>
#include <string>

#ifdef __cplusplus
namespace llvmPy {

enum class ASTType {
    Ignore,
    Expr,
    ExprIdent,
    ExprLit,
    ExprStrLit,
    ExprDecLit,
    ExprIntLit,
    ExprLitAny,
    ExprBinary,
    ExprLambda,
    ExprCall,
    ExprAny,
    Stmt,
    StmtExpr,
    StmtAssign,
    StmtImport,
    StmtDef,
    StmtAny,
    Any,
};

class AST {
public:
    ASTType getType() const { return type; }
    virtual void toStream(std::ostream &) const;
    virtual ~AST() = default;

    static bool classof(AST const *) {
        return true;
    }

protected:
    explicit AST(ASTType type) : type(type) {};

public:
    bool isTypeAbove(ASTType t) const {
        return static_cast<int>(type) >= static_cast<int>(t);
    }

    bool isTypeBelow(ASTType t) const {
        return static_cast<int>(type) < static_cast<int>(t);
    }

    bool isTypeBetween(ASTType a, ASTType b) const {
        return isTypeAbove(a) && isTypeBelow(b);
    }

    bool isType(ASTType t) const {
        return type == t;
    }

private:
    ASTType const type;
};

class Expr : public AST {
public:
    static bool classof(AST const *ast) {
        return ast->isTypeBetween(ASTType::Expr, ASTType::ExprAny);
    }

protected:
    explicit Expr(ASTType type) : AST(type) {}
};

class LitExpr : public Expr {
public:
    static bool classof(AST const *ast) {
        return ast->isTypeBetween(
                ASTType::ExprLit,
                ASTType::ExprLitAny);
    }

protected:
    explicit LitExpr(ASTType type) : Expr(type) {}
};

class StrLitExpr : public LitExpr {
public:
    static bool classof(AST const *ast) {
        return ast->isType(ASTType::ExprStrLit);
    }

    std::string const & str;
    explicit StrLitExpr(std::string str)
        : LitExpr(ASTType::ExprStrLit),
          str(std::move(str)) {}
    void toStream(std::ostream &) const override;
};

class DecLitExpr : public LitExpr {
public:
    static bool classof(AST const *ast) {
        return ast->isType(ASTType::ExprDecLit);
    }

    double const value;
    explicit DecLitExpr(double v)
        : LitExpr(ASTType::ExprDecLit),
          value(v) {}
    void toStream(std::ostream &) const override;
};

class IntLitExpr : public LitExpr {
public:
    static bool classof(AST const *ast) {
        return ast->isType(ASTType::ExprIntLit);
    }

    long const value;
    explicit IntLitExpr(long v)
        : LitExpr(ASTType::ExprIntLit),
          value(v) {}
    void toStream(std::ostream &) const override;
};

class IdentExpr : public Expr {
public:
    static bool classof(AST const *ast) {
        return ast->isType(ASTType::ExprIdent);
    }

    std::string const & name;
    explicit IdentExpr(std::string const * str)
        : Expr(ASTType::ExprIdent),
          name(*str) {}
    void toStream(std::ostream &) const override;
};

class LambdaExpr : public Expr {
public:
    static bool classof(AST const *ast) {
        return ast->isType(ASTType::ExprLambda);
    }

    std::vector<std::string const *> const args;
    Expr const & body;

    explicit LambdaExpr(decltype(args) args, Expr * body)
        : Expr(ASTType::ExprLambda),
          args(std::move(args)),
          body(*body) {}
    void toStream(std::ostream &) const override;
};

class BinaryExpr : public Expr {
public:
    static bool classof(AST const *ast) {
        return ast->isType(ASTType::ExprBinary);
    }

    Expr const & lhs;
    TokenType const op;
    Expr const & rhs;
    BinaryExpr(Expr * lhs, TokenType op, Expr * rhs)
        : Expr(ASTType::ExprBinary),
          lhs(*lhs),
          op(op),
          rhs(*rhs) {}
    void toStream(std::ostream &) const override;
};

class CallExpr : public Expr {
public:
    static bool classof(AST const *ast) {
        return ast->isType(ASTType::ExprCall);
    }

    Expr const &lhs;
    std::vector<Expr const *> const args;
    CallExpr(Expr *lhs, std::vector<Expr const *> args)
        : Expr(ASTType::ExprCall),
          lhs(*lhs),
          args(std::move(args)) {}
    void toStream(std::ostream &) const override;
};

class Stmt : public AST {
public:
    static bool classof(AST const *ast) {
        return ast->isTypeBetween(ASTType::Stmt, ASTType::StmtAny);
    }

protected:
    explicit Stmt(ASTType type) : AST(type) {}
};

class ExprStmt : public Stmt {
public:
    static bool classof(AST const *ast) {
        return ast->isType(ASTType::StmtExpr);
    }

    Expr const & expr;
    explicit ExprStmt(Expr * expr)
        : Stmt(ASTType::StmtExpr),
          expr(*expr) {}
    void toStream(std::ostream &) const override;
};

class AssignStmt : public Stmt {
public:
    static bool classof(AST const *ast) {
        return ast->isType(ASTType::StmtAssign);
    }

    std::string const & lhs;
    Expr const & rhs;
    AssignStmt(std::string const * lhs, Expr * rhs)
        : Stmt(ASTType::StmtAssign),
          lhs(*lhs),
          rhs(*rhs) {}
    void toStream(std::ostream &) const override;
};

class ImportStmt : public Stmt {
public:
    static bool classof(AST const *ast) {
        return ast->isType(ASTType::StmtImport);
    }

    std::string const & modname;
    explicit ImportStmt(std::string const * modname)
        : Stmt(ASTType::StmtImport),
          modname(*modname) {}
    void toStream(std::ostream &) const override;
};

class DefStmt : public Stmt {
public:
    static bool classof(AST const *ast) {
        return ast->isType(ASTType::StmtDef);
    }

    std::string const &name;
    std::vector<std::string const *> args;
    std::vector<Stmt *> const stmts;

    DefStmt(std::string const &name,
            std::vector<std::string const *> args,
            std::vector<Stmt *> stmts)
            : Stmt(ASTType::StmtDef),
              name(name),
              args(std::move(args)),
              stmts(std::move(stmts)) {}

    void toStream(std::ostream &) const override;
};

} // namespace llvmPy

std::ostream & operator<< (std::ostream &, llvmPy::Expr const &);
std::ostream & operator<< (std::ostream &, llvmPy::Stmt const &);

#endif // __cplusplus
