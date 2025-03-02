#include "evoke.h"

bool Evoke::hadError = false;

Evoke::Evoke()
{

}

void Evoke::error(int line, std::string message)
{
	report(line, "", message);
}

void Evoke::error(Token token, std::string message)
{
	if (token.type == END_OF_FILE)
		report(token.line, " at end", message);
	else
		report(token.line, " at '" + token.lexeme + "'", message);
}

void Evoke::runFile(const std::string& path)
{
	std::ifstream myFile(path); //	open file
	if (!myFile.is_open())
		std::cout << "Error opening file" << std::endl;

	std::stringstream buffer;	//	convert to string stream
	buffer << myFile.rdbuf();	//	read string stream into string

	myFile.close();

	run(buffer.str());	//	run the file
	if (hadError)
		exit(EXIT_FAILURE);
}

void Evoke::runPrompt()
{
	std::string line;

	while (true)
	{
		std::cin >> line;	//	get input from user
		if (line == "")
			break;	//	quit if no input
		run(line);	//	run input
		hadError = false; //	reset hadError so that syntax errors don't kill the session
	}
}

void Evoke::run(std::string source)
{
	Scanner scanner(source);
	std::vector<Token> tokens = scanner.scanTokens();
	scanner.printResult();
}

void Evoke::report(int line, std::string location, std::string& message)
{
	std::cout << "[line " << line << "] Error" << location << ": " << message << std::endl;
	hadError = true;
}


