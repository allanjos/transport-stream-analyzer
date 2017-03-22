#ifndef CONFIG_CONFIG_H
#define CONFIG_CONFIG_H

#include <stdio.h>
#include <map>
#include <string>

class Config {
private:

    static Config *instance;

    bool showPacketBinaryInformation;

    std::map<std::string, int> keysInt;
    std::map<std::string, bool> keysBool;
    std::map<std::string, std::string> keysStr;

    Config();

    ~Config();

public:

    static Config *getInstance();

    void Terminate();

    void setKey(std::string name, int value);
    void setKey(std::string name, bool value);
    void setKey(std::string name, std::string value);

    bool hasKey(std::string name);

    int getValueInt(std::string name);
    bool getValueBool(std::string name);
    std::string getValueStr(std::string name);
};

#endif