#include "ASTNode.hpp"

void BinOpNode::create(std::variant<int, std::string> value) {
    this->value = value;
}

SymbolData BinOpNode::evaluate(SymbolTable* table) {
    SymbolData n1 = children[0]->evaluate(table);
    SymbolData n2 = children[1]->evaluate(table);
    SymbolData ans;
    ans.type = DataType::I32_T;

    if (std::get<std::string>(value) == "+" && (n1.type == DataType::I32_T && n2.type == DataType::I32_T))
        ans.data = std::get<int>(n1.data) + std::get<int>(n2.data);
    
    else if (std::get<std::string>(value) == "-")
        ans.data = std::get<int>(n1.data) - std::get<int>(n2.data);
    
    else if (std::get<std::string>(value) == "*")
        ans.data = std::get<int>(n1.data) * std::get<int>(n2.data);
    
    else if (std::get<std::string>(value) == "/")
        ans.data = (int)round(std::get<int>(n1.data) / std::get<int>(n2.data)); 
    
    else if (std::get<std::string>(value) == "==") {
        if (n1.type == DataType::I32_T && n2.type == DataType::I32_T)
            ans.data = (int)(std::get<int>(n1.data) == std::get<int>(n2.data));
        else if (n1.type == DataType::STRING_T && n2.type == DataType::I32_T)
            ans.data = (int)(std::get<std::string>(n1.data) == std::to_string(std::get<int>(n2.data)));
        else if (n1.type == DataType::I32_T && n2.type == DataType::STRING_T)
            ans.data = (int)(std::to_string(std::get<int>(n1.data)) == std::get<std::string>(n2.data));
        else if (n1.type == DataType::STRING_T && n2.type == DataType::STRING_T)
            ans.data = (int)(std::get<std::string>(n1.data) == std::get<std::string>(n2.data));
    }
    
    else if (std::get<std::string>(value) == ">") {
        if (n1.type == DataType::I32_T && n2.type == DataType::I32_T)
            ans.data = (int)(std::get<int>(n1.data) > std::get<int>(n2.data));
        else if (n1.type == DataType::STRING_T && n2.type == DataType::I32_T)
            ans.data = (int)(std::get<std::string>(n1.data).size() > std::get<int>(n2.data));
        else if (n1.type == DataType::I32_T && n2.type == DataType::STRING_T)
            ans.data = (int)(std::get<int>(n1.data) > std::get<std::string>(n2.data).size());
        else if (n1.type == DataType::STRING_T && n2.type == DataType::STRING_T)
            ans.data = (int)(std::get<std::string>(n1.data).size() > std::get<std::string>(n2.data).size());
    }

    else if (std::get<std::string>(value) == "<") {
        if (n1.type == DataType::I32_T && n2.type == DataType::I32_T)
            ans.data = (int)(std::get<int>(n1.data) < std::get<int>(n2.data));
        else if (n1.type == DataType::STRING_T && n2.type == DataType::I32_T)
            ans.data = (int)(std::get<std::string>(n1.data).size() < std::get<int>(n2.data));
        else if (n1.type == DataType::I32_T && n2.type == DataType::STRING_T)
            ans.data = (int)(std::get<int>(n1.data) < std::get<std::string>(n2.data).size());
        else if (n1.type == DataType::STRING_T && n2.type == DataType::STRING_T)
            ans.data = (int)(std::get<std::string>(n1.data).size() <= std::get<std::string>(n2.data).size());
    }
    
    else if (std::get<std::string>(value) == "&&")
        ans.data = std::get<int>(n1.data) > 0 && std::get<int>(n2.data) > 0 ? 1 : 0;
    
    else if (std::get<std::string>(value) == "||")
        ans.data = std::get<int>(n1.data) > 0 || std::get<int>(n2.data) > 0 ? 1 : 0;
    
    else if (std::get<std::string>(value) == ".") {
        std::string leftSide, rightSide;
        if (n1.type == DataType::STRING_T) leftSide = std::get<std::string>(n1.data);
        else leftSide = std::to_string(std::get<int>(n1.data));
        if (n2.type == DataType::STRING_T) rightSide = std::get<std::string>(n2.data);
        else rightSide = std::to_string(std::get<int>(n2.data));

        ans.data = std::string(leftSide + rightSide);
        ans.type = DataType::STRING_T;
    }
    else {
        std::cout << "[ERROR]: Invalid type conversion binary during operation" << std::endl;
        throw 0;
    }
    return ans;
}

void BinOpNode::addChild(ASTNode* child) {
    if (children.size() <= 1)
        children.push_back(child);
    else {
        std::cout << "[ERROR]: Binary operator node can only have two children." << std::endl;
        throw 0;
    }
}

void UnOpNode::create(std::variant<int, std::string> value) {
    this->value = value;
}

SymbolData UnOpNode::evaluate(SymbolTable* table) {
    SymbolData n = children[0]->evaluate(table);
    SymbolData ans;
    ans.type = DataType::I32_T;

    if (std::get<std::string>(value) == "+")
        ans.data = +std::get<int>(n.data);
    else if (std::get<std::string>(value) == "-")
        ans.data = -std::get<int>(n.data);
    else if (std::get<std::string>(value) == "!")
        ans.data = std::get<int>(n.data) == 0 ? 1 : 0;
    return ans;
}

void UnOpNode::addChild(ASTNode* child) {
    if (children.size() == 0)
        children.push_back(child);
    else {
        std::cout << "[ERROR]: Unary operator node can only have one child." << std::endl;
        throw 0;
    }
}

void IntValNode::create(std::variant<int, std::string> value) {
    this->value = value;
}

SymbolData IntValNode::evaluate(SymbolTable* table) {
    SymbolData ans = {
        std::get<int>(value),
        DataType::I32_T
    };
    return ans;
}

void StrValNode::create(std::variant<int, std::string> value) {
    this->value = value;
}

SymbolData StrValNode::evaluate(SymbolTable* table) { 
    SymbolData ans = {
        std::get<std::string>(value),
        DataType::STRING_T
    };
    return ans;   
}

void BlockNode::create(std::variant<int, std::string> value) {}

void BlockNode::addChild(ASTNode* child) {
    children.push_back(child);
}

SymbolData BlockNode::evaluate(SymbolTable* table) {
    for (ASTNode* child : children) {
        SymbolData childData = child->evaluate(table);
        if (childData.isReturn)
            return childData;
    }
    return SymbolData {0, DataType::I32_T};
}

void IdentifierNode::create(std::variant<int, std::string> value) {
    this->value = value;
}

void IdentifierNode::addChild(ASTNode* child) {}

SymbolData IdentifierNode::evaluate(SymbolTable* table) {
    return table->get(std::get<std::string>(this->value));
}

void AssignmentNode::create(std::variant<int, std::string> value) {
    this->value = value;
}

void AssignmentNode::addChild(ASTNode* child) {
    if (children.size() <= 1)
        children.push_back(child);
    else {
        std::cout << "[ERROR]: Assignment node can only have two children." << std::endl;
        throw 0;
    }
}

SymbolData AssignmentNode::evaluate(SymbolTable* table) {
    ASTNode* leftNode = children[0];
    ASTNode* rightNode = children[1];

    SymbolData leftData = table->get(std::get<std::string>(leftNode->value));
    SymbolData rightData = rightNode->evaluate(table);
    table->set(std::get<std::string>(leftNode->value), rightData);
    return SymbolData {0, DataType::I32_T};
}

void PrintNode::create(std::variant<int, std::string> value) {}

void PrintNode::addChild(ASTNode* child) {
    if (children.size() == 0)
        children.push_back(child);
    else {
        std::cout << "[ERROR]: Print node can only have on child." << std::endl;
        throw 0;
    }
}

SymbolData PrintNode::evaluate(SymbolTable* table) {
    SymbolData symbolData = children[0]->evaluate(table);
    if (symbolData.type == DataType::I32_T) std::cout << std::get<int>(symbolData.data) << std::endl;
    else if (symbolData.type == DataType::STRING_T) std::cout << std::get<std::string>(symbolData.data) << std::endl;
    return SymbolData {0, DataType::I32_T};
}

void ReadNode::create(std::variant<int, std::string> value) {
    this->value = value;
}

SymbolData ReadNode::evaluate(SymbolTable* table) {
    std::string n;
    std::cin >> n;
    
    // Check if the user input is an integer number
    std::string::const_iterator nIt = n.begin();
    while (nIt != n.end() && std::isdigit(*nIt)) nIt++;
    if (!n.empty() && nIt == n.end()) {
        SymbolData ans = {
            std::stoi(n),
            DataType::I32_T
        };
        return ans;
    }

    // If not, throw
    else {
        throw 0;
    }
}

void ReadNode::addChild(ASTNode* child) {}

void LoopNode::create(std::variant<int, std::string> value) {
    this->value = value;
}

SymbolData LoopNode::evaluate(SymbolTable* table) {
    // While left node evaluates to true, evaluate the right node
    while (std::get<int>(children[0]->evaluate(table).data) == 1) {
        SymbolData childData = children[1]->evaluate(table);
        if (childData.isReturn)
            return childData;
    }
    return SymbolData {0, DataType::I32_T};
}

void LoopNode::addChild(ASTNode* child) {
    children.push_back(child);
}

void ConditionalNode::create(std::variant<int, std::string> value) {
    this->value = value;
}

SymbolData ConditionalNode::evaluate(SymbolTable* table) {
    // If left node evaluates to true, evaluate the right node
    // Else, if conditional node has 3 childs, evaluate the right node
    if (std::get<int>(children[0]->evaluate(table).data) == 1) {
        SymbolData childData = children[1]->evaluate(table);
        if (childData.isReturn)
            return childData;
    }
    else if (children.size() == 3) {
        SymbolData childData = children[2]->evaluate(table);
        if (childData.isReturn)
            return childData;
    }
    return SymbolData {0, DataType::I32_T};
}

void ConditionalNode::addChild(ASTNode* child) {
    children.push_back(child);
}

void VarDeclarationNode::create(std::variant<int, std::string> value) {}

SymbolData VarDeclarationNode::evaluate(SymbolTable* table) {
    if (hasAssignment) {
        SymbolData assignmentData = children[children.size() - 1]->evaluate(table);
        for (uint32_t i = 0; i < children.size() - 1; i++) {
            table->create(std::get<std::string>(children[i]->value), type);
            table->set(std::get<std::string>(children[i]->value), assignmentData);
        }
    }
    else {
        for (ASTNode* child : children)
            table->create(std::get<std::string>(child->value), type);
    }
    return SymbolData {0, DataType::I32_T};
}

void VarDeclarationNode::addChild(ASTNode* child) {
    children.push_back(child);
}

void FuncDeclarationNode::create(std::variant<int, std::string> value) {
    this->value = value;
}

SymbolData FuncDeclarationNode::evaluate(SymbolTable* table) {
    SymbolData functionData {
        this,
        DataType::FUNCTION_T
    };
    table->create(std::get<std::string>(children[0]->value), DataType::FUNCTION_T);
    table->set(std::get<std::string>(children[0]->value), functionData);
    return functionData;
}

void FuncDeclarationNode::addChild(ASTNode* child) {
    children.push_back(child); 
}

void FuncCallNode::create(std::variant<int, std::string> value) {
    this->value = value;
}

SymbolData FuncCallNode::evaluate(SymbolTable* table) {
    SymbolData callData = globalTable->get(std::get<std::string>(children[0]->value));
    FuncDeclarationNode* decNode = (FuncDeclarationNode*)std::get<void*>(callData.data);

    if (decNode->argumentCount != children.size() - 1) {
        std::cout << "[ERROR]: Invalid number of arguments during function " << std::get<std::string>(children[0]->value) << " call" << std::endl;
        throw 0;        
    }

    std::vector<std::tuple<std::string, DataType>> identifiers;
    SymbolTable* funcTable = new SymbolTable();
    for (uint32_t i = 1; i < decNode->children.size() - 1; i++) {
        VarDeclarationNode* varNode = (VarDeclarationNode*)decNode->children[i];
        varNode->evaluate(funcTable);
        for (ASTNode* child : decNode->children[i]->children)
            identifiers.push_back(std::tuple(std::get<std::string>(child->value), varNode->type));
    }

    for (uint32_t i = 1; i < children.size(); i++) {
        SymbolData argumentData = children[i]->evaluate(table);
        funcTable->set(std::get<std::string>(identifiers[i - 1]), argumentData);
    }

    SymbolData returnData = decNode->children[decNode->children.size() - 1]->evaluate(funcTable);
    if (returnData.isReturn && returnData.type == decNode->returnType)
        return returnData;
    else if (returnData.isReturn) {
        std::cout << "[ERROR]: Function " << std::get<std::string>(children[0]->value) << " return type does not match the function declaration" << std::endl;
        throw 0;
    }

    return SymbolData {0, DataType::I32_T};
}

void FuncCallNode::addChild(ASTNode* child) {
    children.push_back(child); 
}

void ReturnNode::create(std::variant<int, std::string> value) {
    this->value = value;
}

SymbolData ReturnNode::evaluate(SymbolTable* table) {
    SymbolData returnData = children[0]->evaluate(table);
    returnData.isReturn = true;
    return returnData;
}

void ReturnNode::addChild(ASTNode* child) {
    children.push_back(child);
}

void ProgramNode::create(std::variant<int, std::string> value) {
    this->value = value;
}

SymbolData ProgramNode::evaluate(SymbolTable* table) {
    for (ASTNode* child : children)
        child->evaluate(table);
    return SymbolData {0, DataType::I32_T};
}

void ProgramNode::addChild(ASTNode* child) {
    children.push_back(child);
}

void NoOpNode::create(std::variant<int, std::string> value) {}

SymbolData NoOpNode::evaluate(SymbolTable* table) { return SymbolData {0, DataType::I32_T}; }
