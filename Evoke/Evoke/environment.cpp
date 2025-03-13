#include "environment.h"

std::unordered_map<std::string, std::vector<std::unique_ptr<Stmt>>> Environment::envDefinitionMap;

void Environment::defineVariable(const std::string& name, Value value)
{
	values[name] = value;
}

void Environment::assignVariable(Token name, Value value)
{
	if (values.find(name.lexeme) == values.end())
		throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");

	values[name.lexeme] = value;
	return;
}

void Environment::subscribe(const std::string& eventName, std::unique_ptr<Stmt> stmt)
{
	eventMap[eventName].push_back(std::move(stmt));
}

const std::vector<std::unique_ptr<Stmt>>& Environment::getSubscribedStatements(const std::string& eventName) const
{
	std::vector<std::unique_ptr<Stmt>> emptyList;
	auto iterator = eventMap.find(eventName);
	if (iterator == eventMap.end())
	{
		return emptyList;
	}

	return iterator->second;
}

void Environment::createEnvDefinition(Token name, std::vector<std::unique_ptr<Stmt>> stmts)
{
	if (envDefinitionMap.find(name.lexeme) != envDefinitionMap.end())
		throw RuntimeError(name, "Env redefinition '" + name.lexeme + "'.");

	envDefinitionMap[name.lexeme] = std::move(stmts);
}

std::shared_ptr<Environment> Environment::createEnvInstance(Token name)
{
	auto iterator = envDefinitionMap.find(name.lexeme);
	if (iterator == envDefinitionMap.end())
		throw RuntimeError(name, "Undefined environment '" + name.lexeme + "'.");

	auto env = std::make_shared<Environment>();
	for (const auto& stmt : iterator->second)
		//	CONTINUE HERE: FIND A WAY TO PASS INTERPRETER TO ENV AND INIT ENV DECLARATION
		return std::shared_ptr<Environment>();
}

Value Environment::getVariable(Token name) const
{
	if (values.find(name.lexeme) == values.end())
		throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");

	return values.at(name.lexeme);
}

void Environment::defineArray(const std::string& name)
{
	arrayMap[name] = std::vector<Value>();
}

void Environment::pushArray(Token name, Value value)
{
	if (arrayMap.find(name.lexeme) == arrayMap.end())
		throw RuntimeError(name, "Undefined array '" + name.lexeme + "'.");

	arrayMap[name.lexeme].push_back(value);
	return;

}

void Environment::setArrayElement(Token name, int index, Value value)
{
	if (arrayMap.find(name.lexeme) == arrayMap.end())
		throw RuntimeError(name, "Undefined array '" + name.lexeme + "'.");

	if (index >= 0 && index < arrayMap[name.lexeme].size())
	{
		arrayMap[name.lexeme][index] = value;
		return;
	}
	else
	{
		throw RuntimeError(name, "Index out of bounds for array '" + name.lexeme + "'.");
	}


}

Value Environment::getArrayElement(Token name, int index)
{
	if (arrayMap.find(name.lexeme) == arrayMap.end())
		throw RuntimeError(name, "Undefined array '" + name.lexeme + "'.");

	if (index >= 0 && index < arrayMap[name.lexeme].size())
	{
		return arrayMap[name.lexeme][index];;
	}
	else
	{
		throw RuntimeError(name, "Index out of bounds for array '" + name.lexeme + "'.");
	}
}
