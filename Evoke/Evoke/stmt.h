#pragma once
#include "expr.h"
#include <memory>

class StmtVisitor {
public:
	virtual void visit(const class PrintStmt& stmt, bool evoked) const = 0;
	virtual void visit(const class ExpressionStmt& stmt, bool evoked) const = 0;
	virtual void visit(const class ByteStmt& stmt, bool evoked) const = 0;
	virtual void visit(const class EvokeStmt& stmt, bool evoked) const = 0;
};

class Stmt {
public:
	virtual ~Stmt() = default;
	virtual void accept(const StmtVisitor& visitor, bool evoked) const = 0;
	virtual std::unique_ptr<Stmt> clone() const = 0;
};

class PrintStmt : public Stmt {
public:
	Token associatedEvent;
	std::unique_ptr<Expr> expr;

	PrintStmt(Token associatedEvent, std::unique_ptr<Expr> expr)
		: associatedEvent(associatedEvent), expr(std::move(expr)) {}

	void accept(const StmtVisitor& visitor, bool evoked) const override {
		visitor.visit(*this, evoked);
	}

	std::unique_ptr<Stmt> clone() const override {
		// Deep copy the expression
		return std::make_unique<PrintStmt>(associatedEvent, expr ? expr->clone() : nullptr);
	}
};

class ExpressionStmt : public Stmt {
public:
	Token associatedEvent;
	std::unique_ptr<Expr> expr;

	ExpressionStmt(Token associatedEvent, std::unique_ptr<Expr> expr)
		: associatedEvent(associatedEvent), expr(std::move(expr)) {}

	void accept(const StmtVisitor& visitor, bool evoked) const override {
		visitor.visit(*this, evoked);
	}

	std::unique_ptr<Stmt> clone() const override {
		// Deep copy the expression
		return std::make_unique<ExpressionStmt>(associatedEvent, expr ? expr->clone() : nullptr);
	}
};

class ByteStmt : public Stmt {
public:
	Token associatedEvent;
	Token name;
	std::unique_ptr<Expr> initializer;

	ByteStmt(Token associatedEvent, Token name, std::unique_ptr<Expr> initializer)
		: associatedEvent(associatedEvent), name(name), initializer(std::move(initializer)) {}

	void accept(const StmtVisitor& visitor, bool evoked) const override {
		visitor.visit(*this, evoked);
	}

	std::unique_ptr<Stmt> clone() const override {
		// Deep copy the initializer
		return std::make_unique<ByteStmt>(associatedEvent, name, initializer ? initializer->clone() : nullptr);
	}
};

class EvokeStmt : public Stmt {
public:
	Token eventName;
	Token op;
	std::unique_ptr<Expr> condition;

	explicit EvokeStmt(Token eventName, Token op, std::unique_ptr<Expr> condition)
		: eventName(eventName), op(op), condition(std::move(condition)) {}

	void accept(const StmtVisitor& visitor, bool evoked) const override {
		visitor.visit(*this, evoked);
	}

	std::unique_ptr<Stmt> clone() const override {
		// Deep copy the condition
		return std::make_unique<EvokeStmt>(eventName, op, condition ? condition->clone() : nullptr);
	}
};