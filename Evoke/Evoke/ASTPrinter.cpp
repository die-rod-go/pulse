#include "ASTPrinter.h"

std::string ASTPrinter::print(const Expr& expr) {
	result.str("");
	result.clear();
	expr.accept(*this);
	return result.str();
}

void ASTPrinter::visit(const UnaryExpr& expr) const {
	result << "(" << expr.op.lexeme << " ";
	expr.operand->accept(*this);
	result << ")";
}

void ASTPrinter::visit(const BinaryExpr& expr) const {
	result << "(";
	expr.left->accept(*this);
	result << " " << expr.op.lexeme << " ";
	expr.right->accept(*this);
	result << ")";
}

void ASTPrinter::visit(const LiteralExpr& expr) const {
	result << expr.literal.lexeme;
}

void ASTPrinter::visit(const VariableExpr& expr) const {
	result << expr.name.lexeme;
}

void ASTPrinter::visit(const AssignmentExpr& expr) const {
	result << "(set " << expr.name.lexeme << " ";
	expr.value->accept(*this);
	result << ")";
}

void ASTPrinter::visit(const EvokeExpr& expr) const {
	result << "(evoke " << expr.eventName.lexeme << " " << expr.op.lexeme;
	if (expr.condition) {
		result << " ";
		expr.condition->accept(*this);
	}
	result << ")";
}
