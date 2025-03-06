#pragma once
#include "expr.h"
#include "types.h"
#include "errors.h"
#include <iostream>
#include "stmt.h"
#include "environment.h"

class Interpreter : public ExprVisitor, public StmtVisitor
{
public:
	void interpret(std::vector<std::unique_ptr<Stmt>>& statements) const;

	//	exprs
	void visit(const UnaryExpr& expr) const override;
	void visit(const BinaryExpr& expr) const override;
	void visit(const GroupingExpr& expr) const override;
	void visit(const LiteralExpr& expr) const override;
	void visit(const VariableExpr& expr) const override;
	void visit(const AssignmentExpr& expr) const override;

	//	stmts
	void visit(const ExpressionStmt& stmt, bool evoked) const override;
	void visit(const PrintStmt& stmt, bool evoked) const override;
	void visit(const ByteStmt& stmt, bool evoked) const override;
	void visit(const EvokeStmt& stmt, bool evoked) const override;
	void evoke(const EvokeStmt& stmt) const;

private:
	void execute(const Stmt& stmt, bool evoked) const;
	void evaluate(const Expr& expr) const;
	void triggerEvent(const std::string& eventName) const;
	mutable Environment environment;
	mutable byte currentResult;
};