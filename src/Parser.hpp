#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>
#include <iostream>
#include <exception>
#include <cmath>

#include "ASTNode.hpp"
#include "Tokenizer.hpp"

class Parser {
public:
    static Tokenizer tokenizer;
    static SymbolTable symbolTable;

    static ASTNode* parseRelExpression();
    static ASTNode* parseExpression();
    static ASTNode* parseTerm();
    static ASTNode* parseFactor();
    static ASTNode* parseBlock();
    static ASTNode* parseStatement();
    static ASTNode* parseProgram();
    static ASTNode* parseDeclaration();
    static ASTNode* run(std::string code); 
};

#endif
