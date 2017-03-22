#include <iostream>
#include <vector>
#include "application.h"

void Application::setArguments(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        arguments.push_back(argv[i]);
    }
}

char *Application::getArgument(std::string name) {
	for (int i = 0; i < arguments.size(); i++) {
		std::cout << "Argument " << i << std::endl;
		if (std::string(arguments.at(i)) == "-" + name) {
			if (i < arguments.size() - 1) {
			  return arguments.at(i + 1);
		    }
		}
	}

	return NULL;
}

int parseProgramArgs(Application application, int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << "--destination DESTINATION SOURCE" << std::endl;
        return -1;
    }

    return 0;
}