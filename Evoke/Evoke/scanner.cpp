#include "Scanner.h"
#include <bitset>

const std::unordered_map<std::string, TokenType> Scanner::keywords = {
	{"evoke", EVOKE},
	{"clear", CLEAR}
};

Scanner::Scanner(std::string source) : source(source), start(0), current(0), line(1), currentOnLine(0) {}


std::vector<Token> Scanner::scanTokens()
{
	while (!isAtEnd())
	{
		//	at the beginning of the next lexeme
		start = current;
		scanToken();
	}

	tokens.push_back(Token(END_OF_FILE, "", ByteLiteral(), line));
	return tokens;
}

bool Scanner::isAtEnd()
{
	//printCurrent();
	return current >= source.length();
}

void Scanner::scanToken()
{
	//	current character we're looking at
	char c = advance();
	switch (c) {
		//	single character tokens
	case '(': addToken(LEFT_PAREN, ByteLiteral()); break;
	case ')': addToken(RIGHT_PAREN, ByteLiteral()); break;
	case '{': addToken(LEFT_BRACE, ByteLiteral()); break;
	case '}': addToken(RIGHT_BRACE, ByteLiteral()); break;
	case ',': addToken(COMMA, ByteLiteral()); break;
	case '-': addToken(MINUS, ByteLiteral()); break;
	case '+': addToken(PLUS, ByteLiteral()); break;
	case '*': addToken(STAR, ByteLiteral()); break;
	case ':': addToken(COLON, ByteLiteral()); break;
	case ';': addToken(SEMICOLON, ByteLiteral()); break;

		//	checks for tokens that could be single or part of 
		//	double character lexemes ex: ! vs != and > and >=
	case '!':
		addToken(match('=') ? BANG_EQUAL : BANG, ByteLiteral());
		break;
	case '=':
		addToken(match('=') ? EQUAL_EQUAL : EQUAL, ByteLiteral());
		break;
	case '<':
		addToken(match('=') ? LESS_EQUAL : LESS, ByteLiteral());
		break;
	case '>':
		addToken(match('=') ? GREATER_EQUAL : GREATER, ByteLiteral());
		break;
	case '?':
		addToken(match('?') ? QUESTION : QUESTION_QUESTION, ByteLiteral());
		break;

		//	checks for comments "//" and consumes the whole line
	case '/':
		if (match('/')) {
			//	A comment goes until the end of the line.
			while (peek() != '\n' && !isAtEnd())
				advance();
		}
		else {
			addToken(SLASH, ByteLiteral());
		}
		break;

		//	ignore whitespace/meaningless characters
	case ' ':
	case '\r':
	case '\t':
		break;

	case '\n':
		line++;
		currentOnLine = 1;
		break;

	default:
		if (c == '0' || c == '1')
			byteLiteral();
		else if (isalpha(c) || c == '_')
			identifier();
		else
			Evoke::error(line, "Unexpected character: '" + std::string(1, c) + "'");
	}
}

char Scanner::advance()
{
	currentOnLine++;
	return source[current++];
}

void Scanner::addToken(TokenType type, ByteLiteral literal)
{
	int length = getLength(start, current);
	std::string text = source.substr(start, length);
	tokens.push_back(Token(type, text, literal, line));
}

bool Scanner::match(char expected)	//	checks if two characters are equal - conditional advance
{
	if (isAtEnd())
		return false;
	if (source[current] != expected)
		return false;

	currentOnLine++;
	current++;
	return true;
}

char Scanner::peek()
{
	if (isAtEnd())
		return '\0';
	return source[current];
}

char Scanner::peekNext()
{
	if (current + 1 >= source.length())
		return '\0';
	return source[current + 1];
}

void Scanner::byteLiteral()
{
	bool isValidByte = false;

	while (peek() == '0' || peek() == '1')
		advance();

	int length = getLength(start, current);

	//	if it's not a valid byte
	if (length != 8)
	{
		Evoke::error(line, "Invalid byte literal - expected 8 bits.");
		return;
	}

	byte literal = binaryStringToByte(source.substr(start, length));
	addToken(BYTE_LITERAL, ByteLiteral(literal));
}

byte Scanner::binaryStringToByte(const std::string& binaryString)
{
	//	this should never be true but I'll check it just in case
	if (binaryString.length() != 8) {
		Evoke::error(line, "Invalid byte literal - expected 8 bits");
	}
	//	cast and convert to byte
	return static_cast<byte>(std::bitset<8>(binaryString).to_ulong());
}

void Scanner::identifier()
{
	while (isalnum(peek()) || peek() == '_')
		advance();

	int length = getLength(start, current);
	TokenType type;
	std::string text = source.substr(start, length);

	if (keywords.find(text) != keywords.end()) //	if keyword is in map/exists in the language
		type = keywords.at(text);
	else
		type = IDENTIFIER;

	addToken(type, ByteLiteral());
}

int Scanner::getLength(int start, int current)
{
	return current - start;
}

void Scanner::printCurrent()
{
	std::cout << "current: " << current << std::endl;
	std::cout << "line: " << line << std::endl;
	std::cout << "currentOnLine: " << currentOnLine << std::endl;
	std::cout << std::endl;
}

void Scanner::printResult()
{
	for (auto token : tokens)
	{
		std::cout << "====" << std::endl;
		std::cout << token.lexeme << std::endl;
		std::cout << token.type << std::endl;
	}
}