#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>
#include <vector>
#include <variant>

const std::vector<std::string> reservedKeyWords = {
    "while",
    "if",
    "else",
    "var",
    "function",
    "return",
    "begin",
    "end",
    "println"
};

enum TokenType {
    INT,
    STRING,
    PLUS,
    MINUS,
    MULT,
    DIV,
    OPAR,
    CPAR,
    EQUALS,
    IDENTIFIER,
    EQUIVALENCE,
    GREATER,
    LESS,
    OR,
    AND,
    NOT,
    WHILE,
    IF,
    ELSE,
    COMMA,
    VAR,
    FUNCTION,
    RETURN,
    BEGIN,
    END,
    PRINTLN,
    _EOF
};

class Token {
public:
    Token();
    ~Token();

    std::variant<int, std::string> value;
    TokenType type;
};

#endif
