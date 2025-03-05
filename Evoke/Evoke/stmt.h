#pragma once

class StmtVisitor {
public:
    virtual void visit(const class PrintStmt& stmt) const = 0;
    virtual void visit(const class ExpressionStmt& stmt) const = 0;
};

class Stmt
{
public:
	virtual ~Stmt() = default;
	virtual void accept(const StmtVisitor& visitor) const = 0;
};

class PrintStmt : public Stmt 
{
public:
    std::unique_ptr<Expr> expr;
    PrintStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}
    void accept(const StmtVisitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class ExpressionStmt : public Stmt
{
public:
    std::unique_ptr<Expr> expr;
    ExpressionStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}
    void accept(const StmtVisitor& visitor) const override
    {
        visitor.visit(*this);
    }
};