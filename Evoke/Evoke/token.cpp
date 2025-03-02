#include "Token.h"

const std::unordered_map<TokenType, std::string> Token::enumStrings = {
	{LEFT_PAREN, "LEFT_PAREN"}, {RIGHT_PAREN, "RIGHT_PAREN"},
	{LEFT_BRACE, "LEFT_BRACE"}, {RIGHT_BRACE, "RIGHT_BRACE"},
	{COMMA, "COMMA"}, {MINUS, "MINUS"}, {PLUS, "PLUS"}, {STAR, "STAR"},
	{SLASH, "SLASH"}, {COLON, "COLON"}, {SEMICOLON, "SEMICOLON"},

	{BANG, "BANG"}, {BANG_EQUAL, "BANG_EQUAL"},
	{EQUAL, "EQUAL"}, {EQUAL_EQUAL, "EQUAL_EQUAL"},
	{GREATER, "GREATER"}, {GREATER_EQUAL, "GREATER_EQUAL"},
	{LESS, "LESS"}, {LESS_EQUAL, "LESS_EQUAL"},
	{QUESTION, "QUESTION"}, {QUESTION_QUESTION, "QUESTION_QUESTION"},

	{IDENTIFIER, "IDENTIFIER"}, {BYTE_LITERAL, "BYTE"},
	{EVOKE, "EVOKE"}, {CLEAR, "CLEAR"},
	{END_OF_FILE, "END_OF_FILE"}
};

Token::Token(TokenType type, const std::string& lexeme, ByteLiteral literal, int line)
	: type(type), lexeme(lexeme), literal(literal), line(line) {}

std::string Token::toString() const {
	auto it = enumStrings.find(type);
	std::string typeStr = (it != enumStrings.end()) ? it->second : "UNKNOWN";
	return "type: " + typeStr + "\nlexeme: " + lexeme + "\nline: " + std::to_string(line);
}
