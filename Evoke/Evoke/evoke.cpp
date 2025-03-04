#include "evoke.h"
#include "parser.h"
#include "ASTPrinter.h"

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
	//	open file
	std::ifstream myFile(path);
	if (!myFile.is_open())
		std::cout << "Error opening file" << std::endl;

	//	convert to string stream
	std::stringstream buffer;
	//	read string stream into string
	buffer << myFile.rdbuf();

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
		//	get input from user
		std::cin >> line;
		if (line == "")
			break;	//	quit if no input
		//	run input
		run(line);
		//	reset hadError so that syntax errors don't kill the session
		hadError = false;
	}
}

void Evoke::run(std::string source)
{
	Scanner scanner(source);
	std::vector<Token> tokens = scanner.scanTokens();

	Parser parser(tokens);
	std::unique_ptr<Expr> ast = parser.parse();

	if (hadError) return;

	if (ast)
	{
		ASTPrinter printer;
		std::cout << printer.print(*ast) << std::endl;
	}

}

void Evoke::report(int line, std::string location, std::string& message)
{
	std::cout << "[line " << line << "] Error" << location << ": " << message << std::endl;
	hadError = true;
}


