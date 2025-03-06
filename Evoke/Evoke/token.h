#pragma once
#include <string>
#include <unordered_map>
#include "types.h"

enum TokenType {
	// Single-character tokens.
	LEFT_PAREN, RIGHT_PAREN, // ( )
	LEFT_BRACE, RIGHT_BRACE, // { }
	COMMA, MINUS, PLUS, STAR, PERCENT,// , - + * % 
	SLASH, COLON, SEMICOLON, // / : ;

	// One or two character tokens.
	BANG, BANG_EQUAL, // ! !=
	EQUAL, EQUAL_EQUAL, // = ==
	GREATER, GREATER_EQUAL, GREATER_GREATER, // > >= >>
	LESS, LESS_EQUAL, LESS_LESS, // < <= <<
	AND, AND_AND,
	PIPE, PIPE_PIPE,
	QUESTION, QUESTION_QUESTION, // ? ??

	// Literals.
	IDENTIFIER, BYTE_LITERAL,

	// Keywords.
	EMIT, CLEAR, BYTE, PRINT,

	END_OF_FILE,

	NONE
};

class ByteLiteral {
public:
	bool isNull;
	byte value;

	ByteLiteral() : value(0b00000000), isNull(true) {}
	ByteLiteral(byte value) : value(value), isNull(false) {}
};

class Token {
public:
	Token();
	Token(TokenType type, const std::string& lexeme, ByteLiteral literal, int line);
	TokenType type;
	std::string lexeme;
	ByteLiteral literal;
	int line;
	static const std::unordered_map<TokenType, std::string> enumStrings;
	std::string toString() const;

private:
};
