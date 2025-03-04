#pragma once
#include "expr.h"
#include "types.h"

class Interpreter : public ExprVisitor
{
public:
	byte visitLiteralExpr(Expr)
};
