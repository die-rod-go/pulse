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
		if (match('='))
			addToken(LESS_EQUAL, ByteLiteral());
		else if (match('<'))
			addToken(LESS_LESS, ByteLiteral());
		else
			addToken(LESS, ByteLiteral());
		break;
	case '>':
		if (match('='))
			addToken(GREATER_EQUAL, ByteLiteral());
		else if (match('>>'))
			addToken(GREATER_GREATER, ByteLiteral());
		else
			addToken(GREATER, ByteLiteral());
		break;
	case '&':
		addToken(match('&') ? AND_AND : AND, ByteLiteral());
		break;
	case '|':
		addToken(match('|') ? PIPE_PIPE : PIPE, ByteLiteral());
		break;
	case '?':
		addToken(match('?') ? QUESTION_QUESTION : QUESTION, ByteLiteral());
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
	case '0':
		if (match('b'))
		{
			byteLiteral();
		}
		break;

	default:
		if (isdigit(c))
			handleDigit();
		else if (isalpha(c) || c == '_')
			identifier();
		else
			Evoke::error(line, "Unexpected character: '" + std::string(1, c) + "'" + "at: " + std::to_string(currentOnLine));
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

//	checks if two characters are equal - conditional advance
bool Scanner::match(char expected)
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
	//	skip the 0b. only used to calculate the literal value, not the lexeme
	int bitStart = start + 2;

	//	could replace with "while (match('0') || match('1')) {}" but this felt less smelly
	while (peek() == '0' || peek() == '1')
		advance();

	int bitLength = getLength(bitStart, current);

	//	if the bit length is not exactly 8, report an error
	if (bitLength != 8) {
		Evoke::error(line, "Invalid byte literal. Expected 8 bits, got: " + std::to_string(bitLength));
		return;
	}

	//	extract only the 8-bit sequence (excluding "0b")
	std::string bitString = source.substr(bitStart, bitLength);

	//	convert and add the byte literal
	byte literal = binaryStringToByte(bitString);
	addToken(BYTE_LITERAL, ByteLiteral(literal));
}

void Scanner::handleDigit()
{
	while (isdigit(peek()))
		advance();
	int numLength = getLength(start, current);

	//	extract num as string
	std::string numString = source.substr(start, numLength);
	byte numAsByte = std::stoi(numString);
	addToken(BYTE_LITERAL, ByteLiteral(numAsByte));
}

byte Scanner::binaryStringToByte(const std::string& binaryString)
{
	if (binaryString.length() != 8) {
		Evoke::error(line, "Invalid byte literal. Expected 8 bits, got: " + std::to_string(binaryString.length()));
		return 0;
	}
	return static_cast<byte>(std::bitset<8>(binaryString).to_ulong());
}

void Scanner::identifier()
{
	while (isalnum(peek()) || peek() == '_')
		advance();

	int length = getLength(start, current);
	TokenType type;
	std::string text = source.substr(start, length);

	//	if keyword is in map/exists in the language
	if (keywords.find(text) != keywords.end())
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
		std::cout << "==[" + Token::enumStrings.at(token.type) + "]==" << std::endl;
		std::cout << token.lexeme << std::endl;
	}
}