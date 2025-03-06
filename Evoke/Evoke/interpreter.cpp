#include "interpreter.h"
#include "evoke.h"

void Interpreter::interpret(std::vector<std::unique_ptr<Stmt>>& statements) const
{
	try {
		for (const auto& statement : statements)
		{
			execute(*statement, false);
		}
	}
	catch (RuntimeError& error)
	{
		Evoke::runtimeError(error.token, error.message);
	}
}

void Interpreter::execute(const Stmt& stmt, bool evoked) const
{
	stmt.accept(*this, evoked);
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

void Interpreter::visit(const AssignmentExpr& expr) const
{
	evaluate(*expr.value);
	byte value = currentResult;
	environment.assign(expr.name, value);
	currentResult = value;
}

void Interpreter::visit(const GroupingExpr& expr) const
{
	evaluate(*expr.expr);
	currentResult = currentResult;
}

void Interpreter::visit(const ExpressionStmt& stmt, bool evoked) const
{
	if (evoked)
		evaluate(*stmt.expr);
	else
	{
		environment.subscribe(stmt.associatedEvent.lexeme, stmt.clone());
	}
}

void Interpreter::visit(const PrintStmt& stmt, bool evoked) const
{
	if (evoked)
	{
		evaluate(*stmt.expr);
		byte value = currentResult;
		std::cout << (int)value << std::endl;
	}
	else
	{
		environment.subscribe(stmt.associatedEvent.lexeme, stmt.clone());
	}
}

void Interpreter::visit(const ByteStmt& stmt, bool evoked) const
{
	if (evoked)
	{
		byte value = NULL;
		if (stmt.initializer != nullptr)
		{
			evaluate(*stmt.initializer);
			value = currentResult;
		}

		environment.define(stmt.name.lexeme, value);
	}
	else
	{
		environment.subscribe(stmt.associatedEvent.lexeme, stmt.clone());
	}
}

void Interpreter::visit(const EvokeStmt& stmt, bool evoked) const
{
	if (stmt.condition != nullptr)
	{
		evaluate(*stmt.condition);
		bool shouldTrigger = currentResult != 0;
		if (shouldTrigger)
		{
			if (stmt.op.type == QUESTION)
			{
				triggerEvent(stmt.eventName.lexeme);
			}
			else if (stmt.op.type == QUESTION_QUESTION)
			{
				while (shouldTrigger)
				{
					triggerEvent(stmt.eventName.lexeme);
					evaluate(*stmt.condition);
					shouldTrigger = currentResult != 0;
				}
			}
		}
	}
	else
	{
		triggerEvent(stmt.eventName.lexeme);
	}
}

void Interpreter::evaluate(const Expr& expr) const
{
	expr.accept(*this);
}

void Interpreter::triggerEvent(const std::string& eventName) const
{
	const auto& statements = environment.getSubscribedStatements(eventName);
	for (const auto& stmt : statements)
	{
		execute(*stmt, true);
	}
}
