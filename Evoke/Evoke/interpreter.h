#pragma once
#include "expr.h"
#include "types.h"
#include "errors.h"
#include <iostream>
#include "stmt.h"

class Interpreter : public ExprVisitor, public StmtVisitor
{
public:
	void interpret(std::vector<std::unique_ptr<Stmt>>& statements) const;
	void execute(const Stmt& stmt) const;

	//	exprs
	void visit(const UnaryExpr& expr) const override;
	void visit(const BinaryExpr& expr) const override;
	void visit(const GroupingExpr& expr) const override;
	void visit(const LiteralExpr& expr) const override;

	void visit(const VariableExpr& expr) const override {};
	void visit(const AssignmentExpr& expr) const override {};
	void visit(const EvokeExpr& expr) const override {};

	//	stmts
	void visit(const ExpressionStmt& stmt) const override;
	void visit(const PrintStmt& stmt) const override;

private:
	void evaluate(const Expr& expr) const;
	mutable byte currentResult;
};
