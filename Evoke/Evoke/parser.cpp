#include "parser.h"
#include "evoke.h"

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)), current(0) {}

std::vector<std::unique_ptr<Stmt>> Parser::parse()
{
	std::vector<std::unique_ptr<Stmt>> statements;
	try {
		while (!isAtEnd())
		{
			statements.push_back(statement());
		}
	}
	catch (ParseError& error)
	{
		Evoke::error(error.token, error.message);
	}
	return statements;
}

bool Parser::isAtEnd()
{
	return peek().type == END_OF_FILE;
}

Token Parser::advance()
{
	if (!isAtEnd()) current++;
	return previous();
}

Token Parser::peek()
{
	return tokens[current];
}

Token Parser::previous()
{
	return tokens[current - 1];
}

bool Parser::check(TokenType type)
{
	if (isAtEnd()) return false;
	return peek().type == type;
}

std::unique_ptr<Stmt> Parser::statement()
{
	if (match({ PRINT } )) return printStatement();

	return expressionStatement();
}

std::unique_ptr<Stmt> Parser::printStatement()
{
	std::unique_ptr<Expr> value = expression();
	consume(SEMICOLON, "Expect ';' after value.");
	return std::make_unique<PrintStmt>(std::move(value));
}

std::unique_ptr<Stmt> Parser::expressionStatement()
{
	std::unique_ptr<Expr> expr = expression();
	consume(SEMICOLON, "Expect ';' after expression.");
	return std::make_unique<ExpressionStmt>(std::move(expr));
}

bool Parser::match(std::initializer_list<TokenType> types)
{
	for (auto type : types)
	{
		if (check(type))
		{
			advance();
			return true;
		}
	}
	return false;
}

//	consume expected token or report error
Token Parser::consume(TokenType type, std::string message)
{
	if (check(type)) return advance();
	throw ParseError(peek(), message);
	return peek();
}

//	expression -> equality
std::unique_ptr<Expr> Parser::expression()
{
	return equality();
}

//	equality -> comparison ( ( "!=" | "==" ) comparison )
std::unique_ptr<Expr> Parser::equality()
{
	auto expr = comparison();

	while (match({ BANG_EQUAL, EQUAL_EQUAL }))
	{
		Token op = previous();
		auto right = comparison();
		expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
	}

	return expr;
}

//	comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )
std::unique_ptr<Expr> Parser::comparison()
{
	auto expr = term();

	while (match({ GREATER, GREATER_EQUAL, LESS, LESS_EQUAL }))
	{
		Token op = previous();
		auto right = term();
		expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
	}

	return expr;
}

//	term -> factor ( ( "+" | "-" ) factor )
std::unique_ptr<Expr> Parser::term()
{
	auto expr = factor();

	while (match({ PLUS, MINUS }))
	{
		Token op = previous();
		auto right = factor();
		expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
	}

	return expr;
}

// factor -> unary ( ( "*" | "/" ) unary )*
std::unique_ptr<Expr> Parser::factor()
{
	auto expr = unary();

	while (match({ STAR, SLASH }))
	{
		Token op = previous();
		auto right = unary();
		expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
	}

	return expr;
}

//	unary -> ( "!" ) unary | primary
std::unique_ptr<Expr> Parser::unary()
{
	if (match({ BANG }))
	{
		Token op = previous();
		auto right = unary();
		return std::make_unique<UnaryExpr>(op, std::move(right));
	}

	return primary();
}

//	primary -> ByteLiteral | Identifier | "(" expression ")"
std::unique_ptr<Expr> Parser::primary()
{
	if (match({ BYTE_LITERAL }))
		return std::make_unique<LiteralExpr>(previous());

	/*if (match({ IDENTIFIER }))
		return std::make_unique<VariableExpr>(previous());*/

	if (match({ LEFT_PAREN }))
	{
		auto expr = expression();
		consume(RIGHT_PAREN, "Expected ')' after expression");
		return std::make_unique<GroupingExpr>(std::move(expr));
	}

	throw ParseError(peek(), "Expected expression");
	return nullptr;
}

//	syncronization for error recovery
void Parser::synchronize()
{
	advance();

	while (!isAtEnd())
	{
		if (previous().type == SEMICOLON) return;
		switch (peek().type)
		{
		case EVOKE:
		case CLEAR:
		case IDENTIFIER:
			return;
		}
		advance();
	}
}