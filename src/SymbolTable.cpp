#include "SymbolTable.hpp"

SymbolTable::SymbolTable() {}

SymbolTable::~SymbolTable() {}

void SymbolTable::create(std::string symbol, DataType type) {
    if (table.find(symbol) != table.end()) {
        std::cout << "[ERROR]: Symbol " << symbol << " redeclaration not allowed." << std::endl;
        throw 0;
    }
    SymbolData symbolData;
    symbolData.type = type;
    if (type == DataType::I32_T)
        symbolData.data = 0;
    else if (type == DataType::STRING_T)
        symbolData.data = "";
    else if (type == DataType::FUNCTION_T)
        symbolData.data = nullptr;

    table[symbol] = symbolData; 
}

SymbolData SymbolTable::get(std::string symbol) {
    if (table.find(symbol) == table.end()) {
        std::cout << "[ERROR]: Unrecognized symbol " << symbol << ". Declare it before reading a value" << std::endl;
        throw 0;
    }
    return table.at(symbol);
}

void SymbolTable::set(std::string symbol, SymbolData data) {
    std::map<std::string, SymbolData>::iterator it = table.find(symbol);
    if (it == table.end()) {
        std::cout << "[ERROR]: Undeclared symbol " << symbol << ". Declare it before setting a value" << std::endl;
        throw 0;
    }
    else
        it->second = data;
}
