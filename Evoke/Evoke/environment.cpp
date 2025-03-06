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

void Environment::subscribe(const std::string& eventName, std::unique_ptr<Stmt> stmt)
{
	eventMap[eventName].push_back(std::move(stmt));
}

const std::vector<std::unique_ptr<Stmt>>& Environment::getSubscribedStatements(const std::string& eventName) const
{
	std::vector<std::unique_ptr<Stmt>> emptyList;
	auto iterator = eventMap.find(eventName);
	if (iterator != eventMap.end())
	{
		return iterator->second;
	}
	return emptyList;
}

byte Environment::get(Token name) const
{
	if (values.find(name.lexeme) != values.end())
	{
		return values.at(name.lexeme);
	}
	throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
	return byte();
}
