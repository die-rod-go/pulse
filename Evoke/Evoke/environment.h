#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include "types.h"
#include "errors.h"
#include "token.h"
#include "stmt.h"

class Environment
{
private:
	std::unordered_map<std::string, byte> values;
	std::unordered_map<std::string, std::vector<std::unique_ptr<Stmt>>> eventMap;
	std::unordered_map<std::string, std::vector<byte>> arrayMap;

public:
	Environment() {};

	// Variable handling
	void defineVariable(const std::string& name, byte value);
	void assignVariable(Token name, byte value);
	byte getVariable(Token name) const;

	//	Array handling
	void defineArray(const std::string& name);
	void pushArray(Token name, byte value);
	void setArrayElement(Token name, int index, byte value);
	byte getArrayElement(Token name, int index);

	// Event handling
	void subscribe(const std::string& eventName, std::unique_ptr<Stmt> stmt);
	const std::vector<std::unique_ptr<Stmt>>& getSubscribedStatements(const std::string& eventName) const;
};

