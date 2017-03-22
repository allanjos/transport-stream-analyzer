#include "bit.h"
#include "logger.h"
#include <math.h>

std::string Bit::toBinary(short number) {
    std::string str = "";

    for (int index = (sizeof(short) * 8) - 1; index >= 0; index--) {
        short bit = number >> index;

        str += (bit & 1) ? "1" : "0";
    }

    return str;
}

std::string Bit::toBinary(int number) {
    std::string str = "";

    for (int index = (sizeof(int) * 8) - 1; index >= 0; index--) {
        int bit = number >> index;

        str += (bit & 1) ? "1" : "0";
    }

    return str;
}

std::string Bit::toBinary(long number) {
    std::string str = "";

    for (int index = (sizeof(long) * 8) - 1; index >= 0; index--) {
        long bit = number >> index;

        str += (bit & 1) ? "1" : "0";
    }

    return str;
}

std::string Bit::toBinary(long long number) {
    Logger::debug("Bit::toBinary(%lld", number);

    std::string str = "";

    for (int index = (sizeof(long long) * 8) - 1; index >= 0; index--) {
        long long bit = number >> index;

        str += (bit & 1) ? "1" : "0";
    }

    return str;
}