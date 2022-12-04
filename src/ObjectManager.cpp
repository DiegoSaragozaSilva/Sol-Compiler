#include "ObjectManager.hpp"

std::string ObjectManager::file = "";
std::fstream ObjectManager::objectFile = std::fstream();
uint32_t ObjectManager::stackOffset = 0;

void ObjectManager::prepareFile() {
    objectFile.open(file, std::ios_base::out);

    addConstantsToFile();
    addSegmentsToFile();
    addSubroutinesToFile();
    label("_start");
    addHeader();
}

void ObjectManager::closeFile() {
    addFooter();
    objectFile.close();
}

uint32_t ObjectManager::getNextStackOffset() {
    stackOffset += 4;
    return stackOffset;
}

void ObjectManager::moveConstant(std::string reg, uint32_t constant) {
    objectFile << "MOV " + reg + ", " + std::to_string(constant) << std::endl;
}

void ObjectManager::moveRegister(std::string regDst, std::string regSrc) {
    objectFile << "MOV " + regDst + ", " + regSrc << std::endl;
}

void ObjectManager::moveToStack(std::string regSrc, uint32_t stackShift) {
    objectFile << "MOV [EBP - " + std::to_string(stackShift) + "], " + regSrc << std::endl;
}

void ObjectManager::moveFromStack(std::string regDst, uint32_t stackShift) {
    objectFile << "MOV " + regDst + ", [EBP - " + std::to_string(stackShift) + "]" << std::endl;
}

void ObjectManager::stackPush(std::string reg) {
    objectFile << "PUSH " + reg << std::endl;
}

void ObjectManager::stackPop(std::string reg) {
    objectFile << "POP " + reg << std::endl;
}

void ObjectManager::stackAllocate(uint32_t defaultValue) {
    objectFile << "PUSH DWORD " + std::to_string(defaultValue) << std::endl;
}

void ObjectManager::add(std::string regA, std::string regB) {
    objectFile << "ADD " + regA + ", " + regB << std::endl;
}

void ObjectManager::sub(std::string regA, std::string regB) {
    objectFile << "SUB " + regA + ", " + regB << std::endl;
}

void ObjectManager::_and(std::string regA, std::string regB) {
    objectFile << "AND " + regA + ", " + regB << std::endl;
}

void ObjectManager::_or(std::string regA, std::string regB) {
    objectFile << "OR " + regA + ", " + regB << std::endl;
}

void ObjectManager::_xor(std::string regA, std::string regB) {
    objectFile << "XOR " + regA + ", " + regB << std::endl;
}

void ObjectManager::mul(std::string reg) {
    objectFile << "IMUL " + reg << std::endl;
}

void ObjectManager::div(std::string reg) {
    objectFile << "DIV " + reg << std::endl;
}

void ObjectManager::_not(std::string reg) {
    objectFile << "NOT " + reg << std::endl;
}

void ObjectManager::cmp(std::string regA, std::string regB) {
    objectFile << "CMP " + regA + ", " + regB << std::endl;
}

void ObjectManager::call(std::string routine) {
    objectFile << "CALL " + routine << std::endl;
}

void ObjectManager::label(std::string label) {
    objectFile << label + ":" << std::endl;
}

void ObjectManager::jmp(std::string label) {
    objectFile << "JMP " + label << std::endl;
}

void ObjectManager::je(std::string label) {
    objectFile << "JE " + label << std::endl;
}

void ObjectManager::declareConstant(std::string constantName, std::string value) {
    objectFile << constantName + " equ " + value << std::endl;
}

void ObjectManager::addCommentary(std::string text) {
    objectFile << "; " + text << std::endl;
}

void ObjectManager::breakLine() {
    objectFile << std::endl;
}

void ObjectManager::addConstantsToFile() {
    addCommentary("Default compiler constants");
    declareConstant("SYS_EXIT", "1");
    declareConstant("SYS_READ", "3");
    declareConstant("SYS_WRITE", "4");
    declareConstant("STDIN", "0");
    declareConstant("STDOUT", "1");
    declareConstant("True", "1");
    declareConstant("False", "0");
    breakLine();
}

void ObjectManager::addSegmentsToFile() {
    addCommentary("Default segments and sections");
    objectFile << "segment .data" << std::endl;
    breakLine();

    objectFile << "segment .bss" << std::endl;
    objectFile << "res RESB 1" << std::endl;
    breakLine();

    objectFile << "section .text" << std::endl;
    objectFile << "global _start" << std::endl;
    breakLine();
}

void ObjectManager::addSubroutinesToFile() {
    // Print subroutine
    addCommentary("Print subroutine");
    label("print");
    stackPush("EBP");
    moveRegister("EBP", "ESP");
    objectFile << "MOV EAX, [EBP + 8]" << std::endl;
    _xor("ESI", "ESI");
    breakLine();

    // Print dec subroutine
    addCommentary("Print dec subroutine");
    label("print_dec");
    moveConstant("EDX", 0);
    moveRegister("EBX", "0x000A");
    div("EBX");
    add("EDX", "'0'");
    stackPush("EDX");
    objectFile << "INC ESI" << std::endl;
    cmp("EAX", "0");
    objectFile << "JZ print_next" << std::endl;
    jmp("print_dec");
    breakLine();

    // Print next subroutine
    addCommentary("Print next subroutine");
    label("print_next");
    cmp("ESI", "0");
    objectFile << "JZ print_exit" << std::endl;
    objectFile << "DEC ESI" << std::endl;
    moveRegister("EAX", "SYS_WRITE");
    moveRegister("EBX", "STDOUT");
    stackPop("ECX");
    objectFile << "MOV [res], ECX" << std::endl;
    moveRegister("ECX", "res");
    moveConstant("EDX", 1);
    objectFile << "INT 0x80" << std::endl;
    jmp("print_next");
    breakLine();

    // Print exit subroutine
    addCommentary("Print exit subroutine");
    label("print_exit");

    stackPop("EBP");
    objectFile << "RET" << std::endl;
    breakLine();

    // Comparisons helpers
    addCommentary("Comparison subroutines");
    label("binop_je");
    je("binop_true");
    jmp("binop_false");
    breakLine();

    label("binop_jg");
    objectFile << "JG binop_true" << std::endl;
    jmp("binop_false");
    breakLine();

    label("binop_jl");
    objectFile << "JL binop_true" << std::endl;
    jmp("binop_false");
    breakLine();

    label("binop_false");
    moveRegister("EBX", "False");
    jmp("binop_exit");
    breakLine();

    label("binop_true");
    moveRegister("EBX", "True");
    breakLine();

    label("binop_exit");
    objectFile << "RET" << std::endl;
    breakLine();
}

void ObjectManager::addHeader() {
    addCommentary("Header");
    stackPush("EBP");
    moveRegister("EBP", "ESP");
    breakLine();
}

void ObjectManager::addFooter() {
    breakLine();
    addCommentary("Footer");
    stackPop("EBP");
    moveConstant("EAX", 1);
    objectFile << "INT 0x80" << std::endl;
}
