#ifndef _SYMBOLTABLE_H_
#define _SYMBOLTABLE_H_

#include <iostream>
#include <string>
#include <map>
#include <variant>
#include <exception>

enum DataType {
    I32_T,
    STRING_T,
    FUNCTION_T,
    RETURN_T
};

struct SymbolData {
    llvm::Value* data;
    DataType type;
    bool isReturn;
};

class SymbolTable {
public:
    SymbolTable();
    ~SymbolTable();

    void create(std::string symbol, DataType type);
    SymbolData get(std::string symbol);
    void set(std::string symbol, SymbolData data);
private:
    std::map<std::string, SymbolData> table;
};

#endif
