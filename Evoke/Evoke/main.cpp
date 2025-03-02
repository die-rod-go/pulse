#include <iostream>
#include <string>
#include "evoke.h"

int main(int argc, const char* argv[])
{
	Evoke evoke;
	//std::cout << argc << std::endl;

	if (argc == 1) {
		std::cout << "Usage: cproxi [script]\n";
		return EXIT_FAILURE; // Exit after showing usage
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