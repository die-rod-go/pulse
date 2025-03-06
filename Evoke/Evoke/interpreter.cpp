#include "interpreter.h"
#include "evoke.h"

void Interpreter::interpret(std::vector<std::unique_ptr<Stmt>>& statements) const
{
	try {
		for (const auto& statement : statements)
		{
			execute(*statement);
		}
	}
	catch (RuntimeError& error)
	{
		Evoke::runtimeError(error.token, error.message);
	}
}

void Interpreter::execute(const Stmt& stmt) const
{
	stmt.accept(*this);
}

void Interpreter::visit(const UnaryExpr& expr) const
{
	evaluate(*expr.operand);
	byte right = currentResult;

	switch (expr.op.type)
	{
	case (BANG):
		currentResult = ~right;
		break;
	}
}

void Interpreter::visit(const BinaryExpr& expr) const
{
	evaluate(*expr.left);
	byte left = currentResult;

	evaluate(*expr.right);
	byte right = currentResult;

	switch (expr.op.type)
	{
	case MINUS:
		currentResult = left - right;
		break;
	case SLASH:
		if (right == 0)
			throw RuntimeError(expr.op, "Division by zero.");
		currentResult = left / right;
		break;
	case STAR:
		currentResult = left * right;
		break;
	case PLUS:
		currentResult = left + right;
		break;
	case GREATER:
		currentResult = left > right;
		break;
	case GREATER_EQUAL:
		currentResult = left >= right;
		break;
	case LESS:
		currentResult = left < right;
		break;
	case LESS_EQUAL:
		currentResult = left <= right;
		break;
	case BANG_EQUAL:
		currentResult = left != right;
		break;
	case EQUAL_EQUAL:
		currentResult = left == right;
		break;
	}
}

void Interpreter::visit(const LiteralExpr& expr) const
{
	currentResult = expr.literal.literal.value;
}

void Interpreter::visit(const VariableExpr& expr) const
{
	currentResult = environment.get(expr.name);
}

void Interpreter::visit(const GroupingExpr& expr) const
{
	evaluate(*expr.expr);
	currentResult = currentResult;
}

void Interpreter::evaluate(const Expr& expr) const
{
	expr.accept(*this);
}

void Interpreter::visit(const ExpressionStmt& stmt) const 
{
	evaluate(*stmt.expr);
}

void Interpreter::visit(const PrintStmt& stmt) const 
{
	evaluate(*stmt.expr);
	byte value = currentResult;
	std::cout << (int)value << std::endl;
}

void Interpreter::visit(const ByteStmt& stmt) const
{
	byte value = NULL;
	if (stmt.initializer != nullptr)
	{
		evaluate(*stmt.initializer);
		value = currentResult;
	}

	environment.define(stmt.name.lexeme, value);
}
