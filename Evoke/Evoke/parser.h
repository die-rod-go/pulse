#pragma once
#include <vector>
#include <memory>
#include "token.h"
#include "expr.h"

class Parser
{
public:
	explicit Parser(std::vector<Token> tokens);
	std::unique_ptr<Expr> parse();
private:
	const std::vector<Token> tokens;
	int current = 0;

	bool isAtEnd();
	Token advance();
	Token peek();
	Token previous();
	bool match(std::initializer_list<TokenType> types);
	bool check(TokenType type);

	Token consume(TokenType type, std::string message);
	void synchronize();

	std::unique_ptr<Expr> expression();
	std::unique_ptr<Expr> equality();
	std::unique_ptr<Expr> comparison();
	std::unique_ptr<Expr> term();
	std::unique_ptr<Expr> factor();
	std::unique_ptr<Expr> unary();
	std::unique_ptr<Expr> primary();
};