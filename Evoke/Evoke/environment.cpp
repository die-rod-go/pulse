#include "environment.h"

void Environment::define(const std::string& name, byte value)
{
	values[name] = value;
}

void Environment::assign(Token name, byte value)
{
	if (values.find(name.lexeme) != values.end())
	{
		values[name.lexeme] = value;
		return;
	}

	throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");

}

byte Environment::get(Token name)
{
	if(values.find(name.lexeme) != values.end())
	{
		return values.at(name.lexeme);
	}
	throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
	return byte();
}
