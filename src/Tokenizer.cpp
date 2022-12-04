#include "Tokenizer.hpp"

Tokenizer::Tokenizer() {}

Tokenizer::~Tokenizer() {}

void Tokenizer::selectNext() {
    std::variant<int, std::string> tokenValue;
    TokenType tokenType;

    char c = source[position++];
    while (isspace(c) || c == '\n')
        c = source[position++];

    if (c == '+') {
        tokenValue = "+";
        tokenType = TokenType::PLUS;
    }

    else if (c == '-') {
        tokenValue = "-";
        tokenType = TokenType::MINUS;
    }

    else if (c == '*') {
        tokenValue = "*";
        tokenType = TokenType::MULT;
    }

    else if (c == '/') {
        tokenValue = "/";
        tokenType = TokenType::DIV;
    }
    
    else if (c == '(') {
        tokenValue = "(";
        tokenType = TokenType::OPAR;
    }
    
    else if (c == ')') {
        tokenValue = ")";
        tokenType = TokenType::CPAR;
    }

    else if (c == '=') {
        c = source[position];
        if (c == '=') {
            tokenValue = "==";
            tokenType = TokenType::EQUIVALENCE;
            position++;
        }
        else {
            tokenValue = "=";
            tokenType = TokenType::EQUALS;
        }
    }

    else if (c == '>') {
        tokenValue = ">";
        tokenType = TokenType::GREATER;
    }

    else if (c == '<') {
        tokenValue = "<";
        tokenType = TokenType::LESS;
    }

    else if (c == '!') {
        tokenValue = "!";
        tokenType = TokenType::NOT;
    }

    else if (c == '|') {
        c = source[position];
        if (c == '|') {
            tokenValue = "||";
            tokenType = TokenType::OR;
            position++;
        }
        else {
            std::cout << "Missing '|' character in or operator at position " << position + 1 << std::endl;
            throw 0;
        }
    }

    else if (c == '&') {
        c = source[position];
        if (c == '&') {
            tokenValue = "&&";
            tokenType = TokenType::AND;
            position++;
        }
        else {
            std::cout << "Missing '&' character in and operator at position " << position + 1 << std::endl;
            throw 0;
        }
    }

    else if (c == ',') {
        tokenValue = ",";
        tokenType = TokenType::COMMA;
    }

    else if (isalpha(c)) {
        std::string wordString;
        wordString += c;
        tokenType = TokenType::IDENTIFIER;

        c = source[position];
        while (isalnum(c) || c == '_') {
            wordString += c;
            position++;
            c = source[position];
        }

        // Reserved cases
        for (const auto keyWord : reservedKeyWords)
            if (keyWord == wordString) {
                if (keyWord == "while") {
                    tokenType = TokenType::WHILE;
                    break;
                }
                else if (keyWord == "if") {
                    tokenType = TokenType::IF;
                    break;
                }
                else if (keyWord == "else") {
                    tokenType = TokenType::ELSE;
                    break;
                }
                else if (keyWord == "var") {
                    tokenType = TokenType::VAR;
                    break;
                }
                else if (keyWord == "function") {
                    tokenType = TokenType::FUNCTION;
                    break;
                }
                else if (keyWord == "return") {
                    tokenType = TokenType::RETURN;
                    break;
                }
                else if (keyWord == "begin") {
                    tokenType = TokenType::BEGIN;
                    break;
                }
                else if (keyWord == "end") {
                    tokenType = TokenType::END;
                    break;
                }
                else if (keyWord == "println") {
                    tokenType = TokenType::PRINTLN;
                    break;
                }
            }
        tokenValue = wordString;
    }

    else if (c == '"') {
        std::string wordStr = "";
        tokenType = TokenType::STRING;
        
        c = source[position];
        while (c != '"') {
            if (c == '\0') {
                std::cout << "[ERROR]: Missing string closing character at position " << position - 1 << std::endl;
                throw 0;
            }
            wordStr += c;
            position++;
            c = source[position];
        }
        position++;
        tokenValue = wordStr;
    }

    else if (isdigit(c)) {
        tokenValue = (c - '0'); 
        tokenType = TokenType::INT;
        
        c = source[position];
        while (isdigit(c) && !isspace(c)) { 
            tokenValue = std::get<int>(tokenValue) * 10;
            tokenValue = std::get<int>(tokenValue) +  (c - '0');
            position++;
            c = source[position];
        }
    }

    else if (c == '\0') {
        tokenValue = '\0';
        tokenType = TokenType::_EOF;
    }

    else {
        std::cout << "[ERROR]: Unknown token at character " << position - 1 << std::endl;
        throw 0;
    }

    next = Token();
    next.value = tokenValue;
    next.type = tokenType;
}   
