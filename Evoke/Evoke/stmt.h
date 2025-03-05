#pragma once

class StmtVisitor {
public:
};

class Stmt
{
public:
	virtual ~Stmt() = default;
	virtual void accept(const StmtVisitor& visitor) const = 0;
};

