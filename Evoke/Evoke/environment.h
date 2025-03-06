#pragma once
#include <unordered_map>
#include <string>
#include "types.h"
#include "errors.h"
#include "token.h"

class Environment
{
public:
	Environment() {};
	void define(const std::string& name, byte value);
	byte get(Token name);

private:
	std::unordered_map<std::string, byte> values;

};

