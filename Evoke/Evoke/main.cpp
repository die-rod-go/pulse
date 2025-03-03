#include <iostream>
#include <string>
#include "evoke.h"

int main(int argc, const char* argv[])
{
	Evoke evoke;

	if (argc == 1) {
		std::cout << "Usage: evoke [script]\n";
		return EXIT_FAILURE;
	}

	if (argc == 2) {
		evoke.runFile(std::string(argv[1]));
	}
	else {
		evoke.runPrompt();
	}

	std::cout << "quit\n";

	std::cin.get();
	return EXIT_SUCCESS;
}