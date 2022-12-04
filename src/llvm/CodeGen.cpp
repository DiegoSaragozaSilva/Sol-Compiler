#include "Node.hpp"
#include "CodeGen.hpp"
#include "SolParser.hpp"

void CodeContext::generateCode(BlockNode& root) {
    std::cout << "Generating code..." << std::endl;

    std::vector<const llvm::Type*> argTypes;
    llvm::FunctionType& functionType = llvm::FunctionType::get(llvm::Type::getVoidTy(llvm::getGlobalContext()), argTypes, false);
    main = llvm::Function::Create(functionType, llvm::GlobalValue::InternalLinkage, "main", module);
    llvm::BasicBlock* basicBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", main, 0);

    pushBlock(basicBlock);
    root.codeGen(*this);
    llvm::ReturnInst::Create(llvm::getGlobalContext(), basicBlock);
    popBlock();

    std::cout << "Code successfully generated!" << std::endl;

    llvm::PassManager passManager;
    passManager.add(llvm::createPrintModulePass(&llvm::outs()));
    passManager.run(*module);
}

llvm::GenericValue CodeContext::runCode() {
    std::cout << "Running code..." << std::endl;
    llvm::ExistingModuleProvider* moduleProvider = new llvm::ExistingModuleProvider(module);
    llvm::ExecutionMachine* executionMachine = new llvm::ExecutionMachine::create(moduleProvider, false);
    std::vector<llvm::GenericValue> noArgs;
    llvm::GenericValue value = executionMachine->runFunction(main, noArgs);
    std::cout << "Code ran successfully!" << std::endl;

    return value;
}

llvm::Value* IntValNode::evaluate(CodeContext& context) {
    int integer = std::get<int>(value);
    std::cout << "Creating integer byte code for " << integer << "..." << std::endl;

    return llvm::ConstantInt::get(llvm::Type::getInt64Ty(llvm::getGlobalContext()), integer, true);
}

//llvm::Value* StrValNode::evaluate(CodeContext& context) {
//    std::string string = std::get<std::string>(value);
//    std::cout << "Creating string byte code for " << string << "..." << std::endl;
//
//    // [TODO] Implement String to llvm::Value*
//    return llvm::StringRef(string);
//}

llvm::Value* IdentifierNode::evaluate(CodeContext& context) {
    std::string identifier = std::get<std::string>(value);
    std::cout << "Creating identifier byte code for " << identifier << "..." << std::endl;

    return new llvm::LoadInst(context.symbolTable().get(identifier), "", false, context.currentBlock());
}

llvm::Value* BinOpNode::evaluate(CodeContext& context) {
    std::string operation = std::get<std::string>(value);
    std::cout << "Generating byte code for binary operator " << operation << "..." << std::endl;
   
    llvm::Instruction::BinaryOps instruction;
    switch (operation) {
        case PLUS:
            instruction = llvm::Instruction::Add;
            goto binReturn;
            break;
        case MINUS:
            instruction = llvm::Instruction::Sub;
            goto binReturn;
            break;
        case MUL:
            instruction = llvm::Instruction::Mul;
            goto binReturn;
            break;
        case DIV:
            instruction = llvm::Instruction::Div;
            goto binReturn;
            break;
    }

    return nullptr;

    binReturn:
        return llvm::BinaryOperator::Create(instruction, children[0]->evaluate(context), children[1]->evaluate(context), "", context.currentBlock());
}

llvm::Value* BlockNode::evaluate(CodeContext& context) {
    std::cout << "Generating byte code for block..." << std::endl;
    
    llvm::Value* lastValue = nullptr;
    NodeList::const_iterator it;
    for (it = children.begin(); it != children.end(); it++)
        last = (**it).evaluate(context);
    return last;
}

llvm::Value* ProgramNode::evaluate(CodeContext& context) {
    std::cout << "Generating byte code for program..." << std::endl;
    
    llvm::Value* lastValue = nullptr;
    NodeList::const_iterator it;
    for (it = children.begin(); it != children.end(); it++)
        last = (**it).evaluate(context);
    return last;
}

llvm::Value* VarDeclarationNode::evaluate(CodeContext& context) {
    std::string varName = std::get<std::string>(children[0]->value);
    std::cout << "Creating variable declaration byte code for " << varName << std::endl;

    llvm::AllocaInst* alloc = new llvm::AllocaInst(Type::getInt64Ty(getGlobalContext()), varName.c_str(), context.currentBlock());
    context.symbolTable().create(varName, DataType::I32_T);
    return alloc;
}

llvm::Value* AssignmentNode(CodeContext& context) {
    std::string varName = std::get<std::string>(children[0]->value);
    std::cout << "Creating variable assignment byte code for " << varName << std::endl;

    return new llvm::StoreInst(children[1]->evaluate(context), context.symbolTable().get(varName).data, false, context.currentBlock());
}

llvm::Value* FuncCallNode::evaluate(CodeContext& context) {
    std::string functionName = std::get<std::string>(this->value);

    llvm::Function *function = context.module->getFunction(functionName.c_str());
    if (function == NULL) {
        std::cout << "No function called " functionName << " defined" << std::endl;
        throw 0;
    }

    std::vector<llvm::Value*> args;
    llvm::ExpressionList::const_iterator it;
    for (it = arguments.begin(); it != arguments.end(); it++) {
        args.push_back((**it).codeGen(context));
    }

    llvm::CallInst *call = llvm::CallInst::Create(function, args.begin(), args.end(), "", context.currentBlock());
    std::cout << "Creating function call byte code for " << functionName << std::endl;
    return call;
}
