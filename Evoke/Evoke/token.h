#pragma once
#include <string>
#include <unordered_map>
#include "types.h"

enum TokenType {
	// Single-character tokens.
	LEFT_PAREN, RIGHT_PAREN, // ( )
	LEFT_BRACE, RIGHT_BRACE, // { }
	COMMA, MINUS, PLUS, STAR, // , - + * 
	SLASH, COLON, SEMICOLON, // / : ;

	// One or two character tokens.
	BANG, BANG_EQUAL, // ! !=
	EQUAL, EQUAL_EQUAL, // = ==
	GREATER, GREATER_EQUAL, GREATER_GREATER, // > >= >>
	LESS, LESS_EQUAL, LESS_LESS, // < <= <<
	QUESTION, QUESTION_QUESTION, // ? ??

	// Literals.
	IDENTIFIER, BYTE_LITERAL,

	// Keywords.
	EVOKE, CLEAR,

	END_OF_FILE
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
	const TokenType type;
	const std::string lexeme;
	const ByteLiteral literal;
	const int line;

	Token(TokenType type, const std::string& lexeme, ByteLiteral literal, int line);
	std::string toString() const;
	static const std::unordered_map<TokenType, std::string> enumStrings;

private:
};
