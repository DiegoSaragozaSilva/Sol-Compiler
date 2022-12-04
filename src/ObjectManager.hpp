#ifndef _OBJECTMANAGER_H_
#define _OBJECTMANAGER_H_

#include <iostream>
#include <string>
#include <fstream>

class ObjectManager {
public:
    static std::string file;
    static std::fstream objectFile;
    static uint32_t stackOffset;

    static void prepareFile();
    static void closeFile();

    static uint32_t getNextStackOffset();

    static void moveConstant(std::string reg, uint32_t constant);
    static void moveRegister(std::string regDst, std::string regSrc);
    static void moveToStack(std::string regSrc, uint32_t stackShift);
    static void moveFromStack(std::string regDst, uint32_t stackShift);
    static void stackPush(std::string reg);
    static void stackPop(std::string reg);
    static void stackAllocate(uint32_t defaultValue);
    static void add(std::string regA, std::string regB);
    static void sub(std::string regA, std::string regB);
    static void _and(std::string regA, std::string regB);
    static void _or(std::string regA, std::string regB);
    static void _xor(std::string regA, std::string regB);
    static void mul(std::string reg);
    static void div(std::string reg);
    static void _not(std::string reg);
    static void cmp(std::string regA, std::string regB);
    static void call(std::string routine);
    static void label(std::string label);
    static void jmp(std::string label);
    static void je(std::string label);
    static void declareConstant(std::string constantName, std::string value);
    static void addCommentary(std::string text);
    static void breakLine();

private:
    ObjectManager();

    static void addConstantsToFile();
    static void addSegmentsToFile();
    static void addSubroutinesToFile();
    static void addHeader();
    static void addFooter();
};

#endif
