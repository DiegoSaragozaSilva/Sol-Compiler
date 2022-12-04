#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <iostream>
#include <string>
#include <cctype>
#include <exception>

#include "Token.hpp"

class Tokenizer {
public:
    Tokenizer();
    ~Tokenizer();

    int position;
    std::string source;
    Token next;

    void selectNext();
};

#endif
