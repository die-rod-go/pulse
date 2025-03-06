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
public:
	Environment() {};

	// Variable handling
	void define(const std::string& name, byte value);
	void assign(Token name, byte value);
	byte get(Token name) const;

	// Event handling
	void subscribe(const std::string& eventName, std::unique_ptr<Stmt> stmt);
	const std::vector<std::unique_ptr<Stmt>>& getSubscribedStatements(const std::string& eventName) const;

private:
	std::unordered_map<std::string, byte> values;
	std::unordered_map<std::string, std::vector<std::unique_ptr<Stmt>>> eventMap;
};

