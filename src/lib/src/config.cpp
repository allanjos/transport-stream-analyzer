#include "config.h"

Config *Config::instance = NULL;

Config::Config() {
}

Config::~Config() {
}

void Config::Terminate() {
    if (instance != NULL) {
        delete instance;
    }
}

Config *Config::getInstance() {
    if (instance == NULL) {
        instance = new Config();
    }

    return instance;
}

void Config::setKey(std::string name, int value) {
    keysInt[name] = value;
}

void Config::setKey(std::string name, bool value) {
    keysBool[name] = value;   
}

void Config::setKey(std::string name, std::string value) {
    keysStr[name] = value;
}

bool Config::hasKey(std::string name) {
    if (keysInt.find(name) != keysInt.end())
        return true;

    if (keysBool.find(name) != keysBool.end())
        return true;

    if (keysStr.find(name) != keysStr.end())
        return true;

    return false;
}

int Config::getValueInt(std::string name) {
    std::map<std::string, int>::iterator i = keysInt.find(name);

    return i->second;
}

bool Config::getValueBool(std::string name) {
    std::map<std::string, bool>::iterator i = keysBool.find(name);

    return i->second;
}

std::string Config::getValueStr(std::string name) {
    std::map<std::string, std::string>::iterator i = keysStr.find(name);

    return i->second;
}