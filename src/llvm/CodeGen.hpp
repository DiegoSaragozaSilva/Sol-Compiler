#include <stack>
#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Type.h>
#include <llvm/DerivedTypes.h>
#include <llvm/LLVMContext.h>
#include <llvm/PassManager.h>
#include <llvm/Instructions.h>
#include <llvm/CallingConv.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Assembly/PrintModulePass.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/ModuleProvider.h>
#include <llvm/Target/TargetSelect.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/Support/raw_ostream.h>

#include <SymbolTable.hpp>

class BlockNode;

class CodeBlock {
public:
    llvm::BasicBlock* basicBlock;
    SymbolTable symbolTable;
};

class CodeContext {
    std::stack<CodeBlock*> codeBlocks;
    llvm::Function* main;

public:
    llvm::Module* module;
    CodeContext() { module = new llvm::Module("main", llvm::getGlobalContext()); }

    void generateCode(BlockNode& root);
    llvm::GenericValue runCode();
    SymbolTable& symbolTable() { return codeBlocks.top()->symbolTable; }
    llvm::BasicBlock* currentBlock() { return codeBlocks.top()->basicBlock; }
    void pushBlock(llvm::BasicBlock* block) { codeBlocks.push(new CodeBlock()); blocks.top()->block = block; }
    void popBlock() { CodeBlock* top = codeBlocks.top(); codeBlocks.pop(); delete top; }
};
