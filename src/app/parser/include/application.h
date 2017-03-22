#ifndef APPLICATION_H
#define APPLICATION_H

class Application {
private:
    std::vector<char *> arguments;

public:
    void setArguments(int argc, char *argv[]);

    char *getArgument(std::string name);
};

#endif // APPLICATION_H