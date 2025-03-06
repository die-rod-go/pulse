#include "environment.h"

void Environment::define(const std::string& name, byte value)
{
	values[name] = value;
}

byte Environment::get(Token name)
{
	if(values.find(name.lexeme) != values.end())
	{
		return values.at(name.lexeme);
	}
	throw new RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
	return byte();
}
