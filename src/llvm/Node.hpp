#ifndef _ASTNODE_H_
#define _ASTNODE_H_

#include <iostream>
#include <exception>
#include <cmath>
#include <vector>
#include <variant>
#include <typeinfo>
#include <llvm/IR/Value.h>

#include "SymbolTable.hpp"

class ASTNode {
public:
    ASTNode() {}
    virtual ~ASTNode() {}

    void create(std::variant<int, std::string> value) {this->value = value;}
    void addChild(ASTNode* child) {this->children.push_back(child);}

    virtual llvm::Value* evaluate(CodeContext& context) = 0;

    std::variant<int, std::string> value;
    std::vector<ASTNode*> children;
};

class BinOpNode : public ASTNode {
public:
    ~BinOpNode() {}
    llvm::Value* evaluate(CodeContext& context);
};

class IntValNode : public ASTNode {
public:
    llvm::Value* evaluate(CodeContext& context);
};

//class StrValNode : public ASTNode {
//public:
//    llvm::Value* evaluate(CodeContext& context);
//};

class BlockNode : public ASTNode {
public:
    llvm::Value* evaluate(CodeContext& context);
};

class IdentifierNode : public ASTNode {
public:
    llvm::Value* evaluate(CodeContext& context);
};

class AssignmentNode : public ASTNode {
public:
    llvm::Value* evaluate(CodeContext& context);
};

class LoopNode : public ASTNode {
public:
    llvm::Value* evaluate(CodeContext& context);
};

class ConditionalNode : public ASTNode {
public:
    llvm::Value* evaluate(CodeContext& context);
};

class VarDeclarationNode : public ASTNode {
public:
    DataType type;
    
    llvm::Value* evaluate(CodeContext& context);
};

class FuncDeclarationNode : public ASTNode {
public:
    uint32_t argumentCount;
    DataType returnType;

    llvm::Value* evaluate(CodeContext& context);
};

class FuncCallNode : public ASTNode {
public: 
    SymbolTable* globalTable;

    llvm::Value* evaluate(CodeContext& context);
};

class ReturnNode : public ASTNode {
public:
    void* __aux__; 

    llvm::Value* evaluate(CodeContext& context);
};

class ProgramNode : public ASTNode {
public: 
    llvm::Value* evaluate(CodeContext& context); 
};

class NoOpNode : public ASTNode {
public:
    llvm::Value* evaluate(CodeContext& context);
};

typedef std::vector<ASTNode*> NodeList;

#endif
