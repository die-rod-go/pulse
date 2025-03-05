#include "interpreter.h"
#include "evoke.h"

void Interpreter::interpret(const Expr& expr) const
{
	try {
		evaluate(expr);
		byte value = currentResult;
		std::cout << static_cast<int>(value)<< std::endl;
	}
	catch (RuntimeError error)
	{
		Evoke::runtimeError(error.token, error.message);
	}
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

void Interpreter::visit(const GroupingExpr& expr) const
{
	evaluate(*expr.expr);
	currentResult = currentResult;
}

void Interpreter::evaluate(const Expr& expr) const
{
	expr.accept(*this);
}
