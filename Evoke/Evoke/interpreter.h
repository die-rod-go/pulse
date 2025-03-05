#pragma once
#include "expr.h"
#include "types.h"
#include "errors.h"
#include <iostream>

class Interpreter : public ExprVisitor
{
public:
	void interpret(const Expr& expr) const;

	void visit(const UnaryExpr& expr) const override;
	void visit(const BinaryExpr& expr) const override;
	void visit(const GroupingExpr& expr) const override;
	void visit(const LiteralExpr& expr) const override;

	void visit(const VariableExpr& expr) const override {};
	void visit(const AssignmentExpr& expr) const override {};
	void visit(const EvokeExpr& expr) const override {};

private:
	void evaluate(const Expr& expr) const;
	mutable byte currentResult;
};
