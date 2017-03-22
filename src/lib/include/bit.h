#ifndef BIT_H
#define BIT_H

#include <string>

class Bit {
    public:
        static std::string toBinary(short number);

        static std::string toBinary(int number);

        static std::string toBinary(long number);

        static std::string toBinary(long long number);
};

#endif