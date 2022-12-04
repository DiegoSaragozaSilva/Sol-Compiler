#ifndef _PREPRO_H_
#define _PREPRO_H_

#include <sstream>
#include <string>
#include <iostream>

#include <Token.hpp>

class PrePro {
public:
    static std::string filter(std::string str);
};

#endif
