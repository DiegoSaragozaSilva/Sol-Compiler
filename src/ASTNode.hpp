#ifndef _ASTNODE_H_
#define _ASTNODE_H_

#include <iostream>
#include <exception>
#include <cmath>
#include <vector>
#include <variant>
#include <typeinfo>

#include "SymbolTable.hpp"

class ASTNode {
public:
    virtual void create(std::variant<int, std::string> value) = 0;
    virtual SymbolData evaluate(SymbolTable* table) = 0;
    virtual void addChild(ASTNode* child) = 0;

    std::variant<int, std::string> value;
    std::vector<ASTNode*> children;
};

class BinOpNode : public ASTNode {
public:
    void create(std::variant<int, std::string> value);
    SymbolData evaluate(SymbolTable* table);
    void addChild(ASTNode* child);
};

class UnOpNode : public ASTNode {
public:
    void create(std::variant<int, std::string> value);
    SymbolData evaluate(SymbolTable* table);
    void addChild(ASTNode* child);
};

class IntValNode : public ASTNode {
public:
    void create(std::variant<int, std::string> value);
    SymbolData evaluate(SymbolTable* table);
    void addChild(ASTNode* child) {}
};

class StrValNode : public ASTNode {
public:
    void create(std::variant<int, std::string> value);
    SymbolData evaluate(SymbolTable* table);
    void addChild(ASTNode* child) {}
};

class BlockNode : public ASTNode {
public:
    void create(std::variant<int, std::string> value);
    SymbolData evaluate(SymbolTable* table);
    void addChild(ASTNode* child);
};

class IdentifierNode : public ASTNode {
public:
    void create(std::variant<int, std::string> value);
    SymbolData evaluate(SymbolTable* table);
    void addChild(ASTNode* child);
};

class AssignmentNode : public ASTNode {
public:
    void create(std::variant<int, std::string> value);
    SymbolData evaluate(SymbolTable* table);
    void addChild(ASTNode* child);
};

class PrintNode : public ASTNode {
public:
    void create(std::variant<int, std::string> value);
    SymbolData evaluate(SymbolTable* table);
    void addChild(ASTNode* child);
};

class ReadNode : public ASTNode {
public:
    void create(std::variant<int, std::string> value);
    SymbolData evaluate(SymbolTable* table);
    void addChild(ASTNode* child);
};

class LoopNode : public ASTNode {
public:
    void create(std::variant<int, std::string> value);
    SymbolData evaluate(SymbolTable* table);
    void addChild(ASTNode* child);
};

class ConditionalNode : public ASTNode {
public:
    void create(std::variant<int, std::string> value);
    SymbolData evaluate(SymbolTable* table);
    void addChild(ASTNode* child);
};

class VarDeclarationNode : public ASTNode {
public:
    bool hasAssignment;
    DataType type;
    void create(std::variant<int, std::string> value);
    SymbolData evaluate(SymbolTable* table);
    void addChild(ASTNode* child);
};

class FuncDeclarationNode : public ASTNode {
public:
    uint32_t argumentCount;
    DataType returnType;

    void create(std::variant<int, std::string> value);
    SymbolData evaluate(SymbolTable* table);
    void addChild(ASTNode* child);
};

class FuncCallNode : public ASTNode {
public: 
    SymbolTable* globalTable;

    void create(std::variant<int, std::string> value);
    SymbolData evaluate(SymbolTable* table);
    void addChild(ASTNode* child); 
};

class ReturnNode : public ASTNode {
public:
    void* __aux__;

    void create(std::variant<int, std::string> value);
    SymbolData evaluate(SymbolTable* table);
    void addChild(ASTNode* child); 
};

class ProgramNode : public ASTNode {
public: 
    void create(std::variant<int, std::string> value);
    SymbolData evaluate(SymbolTable* table);
    void addChild(ASTNode* child); 
};

class NoOpNode : public ASTNode {
public:
    void create(std::variant<int, std::string> value);
    SymbolData evaluate(SymbolTable* table);
    void addChild(ASTNode* child) {}
};
#endif
