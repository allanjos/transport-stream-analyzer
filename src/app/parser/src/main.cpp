#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "main.h"
#include "application.h"
#include "ts/ts.h"

Application *application = NULL;

int main(int argc, char *argv[]) {
    std::cout << "Transport Stream Parser" << std::endl;

    std::cout << "Arguments: " << argc << std::endl;

    if (argc < 2) {
        printUsage(argv[0]);
        return -1;
    }

    application = new Application();

    application->setArguments(argc, argv);

    char *tsFilePath = application->getArgument("ts");

    std::cout << "Transport stream file path: " << tsFilePath << std::endl;

    TransportStream *ts = new TransportStream();

    ts->setPath(tsFilePath);

    ts->load();

    terminate();

    return 0;
}


void printUsage(const char *program_name) {
    std::cerr << std::endl;
    std::cerr << "Usage:" << std::endl;
    std::cerr << "%s <OPTIONS>" << std::endl;
    std::cerr << std::endl;
    std::cerr << "OPTIONS:" << std::endl;
    std::cerr << "-ts Transport stream file path." << std::endl;
    std::cerr << std::endl;
}

void terminate() {
    if (application != NULL) {
        delete application;

        application = NULL;
    }
}