#include "Parser.hpp"

Tokenizer Parser::tokenizer = Tokenizer();
SymbolTable Parser::symbolTable = SymbolTable();

ASTNode* Parser::run(std::string code) {
    tokenizer.source = code;
    tokenizer.position = 0;
    tokenizer.selectNext();

    ASTNode* root = parseProgram();
    if (tokenizer.next.type != TokenType::_EOF) {
        std::cout << "[ERROR]: Invalid EOF character at position " << tokenizer.position - 1 << std::endl;
        throw 0;
    }

    IdentifierNode* mainIdentifier = new IdentifierNode();
    mainIdentifier->create("main");
    FuncCallNode* mainCall = new FuncCallNode();
    mainCall->globalTable = &Parser::symbolTable;
    mainCall->addChild(mainIdentifier);
    root->addChild(mainCall);

    return root;
}

ASTNode* Parser::parseFactor() {
    switch (tokenizer.next.type) {
        case TokenType::PLUS:
            {
                // Unary plus
                UnOpNode* unPlus = new UnOpNode();
                unPlus->create("+");

                tokenizer.selectNext(); 
                unPlus->addChild(parseFactor());
                return unPlus; 
            }
        case TokenType::MINUS:
            {
                // Unary minus
                UnOpNode* unMinus = new UnOpNode();
                unMinus->create("-");

                tokenizer.selectNext();
                unMinus->addChild(parseFactor());
                return unMinus;
            }
        case TokenType::NOT:
            {
                // Unary not
                UnOpNode* unNot = new UnOpNode();
                unNot->create("!");

                tokenizer.selectNext();
                unNot->addChild(parseFactor());
                return unNot;
            }
        case TokenType::INT:
            {
                // Int value node
                IntValNode* intValue = new IntValNode();
                intValue->create(std::get<int>(tokenizer.next.value));
                tokenizer.selectNext();
                return intValue; 
            }
        case TokenType::STRING:
            {
                // String value node
                StrValNode* strValue = new StrValNode();
                strValue->create(std::get<std::string>(tokenizer.next.value));
                tokenizer.selectNext();
                return strValue; 
            }
        case TokenType::IDENTIFIER:
            {
                // Identifier node
                IdentifierNode* identifierNode = new IdentifierNode();
                identifierNode->create(std::get<std::string>(tokenizer.next.value));
                tokenizer.selectNext();

                if (tokenizer.next.type == TokenType::OPAR) {
                    FuncCallNode* funcCallNode = new FuncCallNode();
                    funcCallNode->globalTable = &Parser::symbolTable;
                    funcCallNode->addChild(identifierNode);
                    tokenizer.selectNext();
                    if (tokenizer.next.type == TokenType::CPAR) {
                        tokenizer.selectNext();
                        return funcCallNode;
                    }
                    funcCallNode->addChild(parseRelExpression());
                    while (tokenizer.next.type == TokenType::COMMA) {
                        tokenizer.selectNext();
                        funcCallNode->addChild(parseRelExpression());
                    }
                    if (tokenizer.next.type == TokenType::CPAR) {
                        tokenizer.selectNext();
                        return funcCallNode;
                    }
                    else {
                        std::cout << "[ERROR]: Missing closing parenthesis during function call at position " << tokenizer.position - 1 << std::endl;
                        throw 0;
                    }
                }
                else
                    return identifierNode;
            }
        case TokenType::OPAR:
            {
                tokenizer.selectNext();
                
                // Generic node
                ASTNode* genericNode = parseRelExpression(); 
                if (tokenizer.next.type == TokenType::CPAR) {
                    tokenizer.selectNext();
                    return genericNode;
                }
                std::cout << "[ERROR]: Missing closing parenthesis at character " << tokenizer.position - 1 << std::endl;
                throw 0;
            }
        default:
            std::cout << "[ERROR]: Invalid character at position " << tokenizer.position - 1 << std::endl; 
            throw 0;
    }
}

ASTNode* Parser::parseTerm() {
    if (tokenizer.next.type == TokenType::INT || tokenizer.next.type == TokenType::PLUS || tokenizer.next.type == TokenType::MINUS || tokenizer.next.type == TokenType::OPAR || tokenizer.next.type == TokenType::IDENTIFIER || tokenizer.next.type == TokenType::NOT || tokenizer.next.type == TokenType::STRING) {
        ASTNode* node = parseFactor();
        while (tokenizer.next.type == TokenType::MULT || tokenizer.next.type == TokenType::DIV || tokenizer.next.type == TokenType::AND) {
            switch (tokenizer.next.type) {
                case TokenType::MULT:
                    {
                        BinOpNode* _node = new BinOpNode();
                        _node->create("*");
                        _node->addChild(node);
                        tokenizer.selectNext();
                        
                        node = parseFactor();
                        _node->addChild(node);
                        node = _node;
                        break;
                    }
                case TokenType::DIV:
                    {
                        BinOpNode* _node = new BinOpNode();
                        _node->create("/");
                        _node->addChild(node);
                        tokenizer.selectNext();
                        
                        node = parseFactor();
                        _node->addChild(node);
                        node = _node;
                        break;
                    }
                case TokenType::AND:
                    {
                        BinOpNode* _node = new BinOpNode();
                        _node->create("&&");
                        _node->addChild(node);
                        tokenizer.selectNext();
                        
                        node = parseFactor();
                        _node->addChild(node);
                        node = _node;
                        break;
                    }
            }
        }
        return node;
    }
    std::cout << "[ERROR]: Invalid character at position " << tokenizer.position - 1 << std::endl;
    throw 0;
}

ASTNode* Parser::parseRelExpression() {
    if (tokenizer.next.type == TokenType::INT || tokenizer.next.type == TokenType::PLUS || tokenizer.next.type == TokenType::MINUS || tokenizer.next.type == TokenType::OPAR || tokenizer.next.type == TokenType::IDENTIFIER || tokenizer.next.type == TokenType::NOT || tokenizer.next.type == TokenType::STRING) {
        ASTNode* node = parseExpression();
        while (tokenizer.next.type == TokenType::EQUIVALENCE || tokenizer.next.type == TokenType::GREATER || tokenizer.next.type == TokenType::LESS) {
            switch (tokenizer.next.type) {
                case TokenType::EQUIVALENCE:
                    {
                        BinOpNode* _node = new BinOpNode();
                        _node->create("==");
                        _node->addChild(node);
                        tokenizer.selectNext();
                        
                        node = parseExpression();
                        _node->addChild(node);
                        node = _node;
                        break;
                    }
                case TokenType::GREATER:
                    {
                        BinOpNode* _node = new BinOpNode();
                        _node->create(">");
                        _node->addChild(node);
                        tokenizer.selectNext();
                        
                        node = parseExpression();
                        _node->addChild(node);
                        node = _node;
                        break;
                    }
                case TokenType::LESS:
                    {
                        BinOpNode* _node = new BinOpNode();
                        _node->create("<");
                        _node->addChild(node);
                        tokenizer.selectNext();
                        
                        node = parseExpression();
                        _node->addChild(node);
                        node = _node;
                        break;
                    }
            }
        }
        return node; 
    }
    std::cout << "[ERROR]: Invalid character at position " << tokenizer.position - 1 << std::endl;
    throw 0;

}

ASTNode* Parser::parseExpression() {
    if (tokenizer.next.type == TokenType::INT || tokenizer.next.type == TokenType::PLUS || tokenizer.next.type == TokenType::MINUS || tokenizer.next.type == TokenType::OPAR || tokenizer.next.type == TokenType::IDENTIFIER || tokenizer.next.type == TokenType::NOT || tokenizer.next.type == TokenType::STRING) {
        ASTNode* node = parseTerm();
        while (tokenizer.next.type == TokenType::PLUS || tokenizer.next.type == TokenType::MINUS || tokenizer.next.type == TokenType::OR) {
            switch (tokenizer.next.type) {
                case TokenType::PLUS:
                    {
                        BinOpNode* _node = new BinOpNode();
                        _node->create("+");
                        _node->addChild(node);
                        tokenizer.selectNext();
                        
                        node = parseTerm();
                        _node->addChild(node);
                        node = _node;
                        break;
                    }
                case TokenType::MINUS:
                    {
                        BinOpNode* _node = new BinOpNode();
                        _node->create("-");
                        _node->addChild(node);
                        tokenizer.selectNext();
                        
                        node = parseTerm();
                        _node->addChild(node);
                        node = _node;
                        break;
                    }
                case TokenType::OR:
                    {
                        BinOpNode* _node = new BinOpNode();
                        _node->create("||");
                        _node->addChild(node);
                        tokenizer.selectNext();
                        
                        node = parseTerm();
                        _node->addChild(node);
                        node = _node;
                        break;
                    }
            }
        }
        return node;
   
    }
    std::cout << "[ERROR]: Invalid character at position " << tokenizer.position - 1 << std::endl;
    throw 0;
}

ASTNode* Parser::parseStatement() {
    ASTNode* statementNode = nullptr;
    if (tokenizer.next.type == TokenType::IDENTIFIER) {
        IdentifierNode* identifierNode = new IdentifierNode();
        identifierNode->create(std::get<std::string>(tokenizer.next.value));
        tokenizer.selectNext();
        if (tokenizer.next.type == TokenType::EQUALS) {
            statementNode = new AssignmentNode();
            statementNode->create("=");
            statementNode->addChild(identifierNode);            
            tokenizer.selectNext();
            statementNode->addChild(parseRelExpression()); 
        }
        else if (tokenizer.next.type == TokenType::OPAR) {
            FuncCallNode* funcCallNode = new FuncCallNode();
            funcCallNode->globalTable = &Parser::symbolTable;
            funcCallNode->addChild(identifierNode);
            tokenizer.selectNext();
            if (tokenizer.next.type == TokenType::CPAR) {
                tokenizer.selectNext();
                return funcCallNode;
            }
            funcCallNode->addChild(parseRelExpression());
            while (tokenizer.next.type == TokenType::COMMA) {
                tokenizer.selectNext();
                funcCallNode->addChild(parseRelExpression());
            }
            if (tokenizer.next.type == TokenType::CPAR) {
                tokenizer.selectNext();
                return funcCallNode;
            }
        }
        else {
            std::cout << "[ERROR]: Invalid character at position " << tokenizer.position - 1 << std::endl;
            throw 0;
        }

        return statementNode;
    }
    else if (tokenizer.next.type == TokenType::PRINTLN) {
        statementNode = new PrintNode();
        tokenizer.selectNext();
        if (tokenizer.next.type == TokenType::OPAR) {
            tokenizer.selectNext();
            statementNode->addChild(parseRelExpression());
            if (tokenizer.next.type != TokenType::CPAR) {
                std::cout << "[ERROR]: Missing closing parenthesis during print function evocation at position " << tokenizer.position - 1 << std::endl;
                throw 0;
            }
            tokenizer.selectNext();
        }
        else {
            std::cout << "[ERROR]: Missing opening parenthesis during print function evocation at position " << tokenizer.position - 1 << std::endl;
            throw 0;
        }

        return statementNode;
    }
    else if (tokenizer.next.type == TokenType::VAR) {
        tokenizer.selectNext();
        VarDeclarationNode* varNode = new VarDeclarationNode();
        if (tokenizer.next.type == TokenType::IDENTIFIER) {
            IdentifierNode* identifierNode = new IdentifierNode();
            identifierNode->create(std::get<std::string>(tokenizer.next.value));
            varNode->addChild(identifierNode); 
            varNode->hasAssignment = false;
            tokenizer.selectNext();
            if (tokenizer.next.type == TokenType::EQUALS) {
                tokenizer.selectNext();
                varNode->hasAssignment = true;
                varNode->addChild(parseRelExpression());
            }
            else {
                while (tokenizer.next.type == TokenType::COMMA) {
                    tokenizer.selectNext();
                    if (tokenizer.next.type == TokenType::IDENTIFIER) {
                        IdentifierNode* newIdentifierNode = new IdentifierNode();
                        newIdentifierNode->create(std::get<std::string>(tokenizer.next.value));
                        varNode->addChild(newIdentifierNode);
                        tokenizer.selectNext();
                    }
                    else {
                        std::cout << "[ERROR]: Missing symbol identifier during variable declaration after ',' character at position " << tokenizer.position - 1 << std::endl;
                        throw 0;
                    }
                }
            }
            return varNode;
        }
        else {
            std::cout << "[ERROR]: Missing symbol identifier during variable declaration at position << " << tokenizer.position - 1 << std::endl;
            throw 0;
        }
    }
    else if (tokenizer.next.type == TokenType::RETURN) {
        tokenizer.selectNext();
        ASTNode* returnNode = new ReturnNode();
        returnNode->addChild(parseRelExpression());
        return returnNode;
    }

    else if (tokenizer.next.type == TokenType::WHILE) {
        LoopNode* whileNode = new LoopNode();
        whileNode->create("while");
        tokenizer.selectNext();

        if (tokenizer.next.type == TokenType::OPAR) {
            tokenizer.selectNext();
            whileNode->addChild(parseRelExpression());
            if (tokenizer.next.type == TokenType::CPAR) {
                tokenizer.selectNext();
                whileNode->addChild(parseStatement());
                return whileNode;
            }
            std::cout << "[ERROR]: Missing ')' character at position " << tokenizer.position - 1 << std::endl;
            throw 0;
        }
        std::cout << "[ERROR]: Missing '(' character at position " << tokenizer.position - 1 << std::endl;
        throw 0;
    }

    else if (tokenizer.next.type == TokenType::IF) {
        ConditionalNode* ifNode = new ConditionalNode();
        ifNode->create("if");
        tokenizer.selectNext();
        
        if (tokenizer.next.type == TokenType::OPAR) {
            tokenizer.selectNext();
            ifNode->addChild(parseRelExpression());

            if (tokenizer.next.type == TokenType::CPAR) {
                tokenizer.selectNext();
                ifNode->addChild(parseStatement());
            
                if (tokenizer.next.type == TokenType::ELSE) {
                    tokenizer.selectNext();
                    ifNode->addChild(parseStatement());
                }
                return ifNode;
            }
            std::cout << tokenizer.next.type << std::endl;
            std::cout << "[ERROR]: Missing ')' character at position " << tokenizer.position - 1 << std::endl;
            throw 0;
        }
        std::cout << "[ERROR]: Missing '(' character at position " << tokenizer.position - 1 << std::endl;
        throw 0;
    }

    else return parseBlock();    
}

ASTNode* Parser::parseBlock() {
    BlockNode* blockNode = new BlockNode();
    if (tokenizer.next.type == TokenType::BEGIN) {
        tokenizer.selectNext(); 
        while (tokenizer.next.type != TokenType::END)
            blockNode->addChild(parseStatement());
        tokenizer.selectNext();
    }
    else {
        std::cout << "[ERROR]: No block opening character 'begin' at position " << tokenizer.position - 1 << std::endl;
        throw 0;
    }
    return blockNode;
}

ASTNode* Parser::parseDeclaration() {
    FuncDeclarationNode* funcDeclarationNode = new FuncDeclarationNode();
    if (tokenizer.next.type == TokenType::FUNCTION) {
        funcDeclarationNode->create("function");
        tokenizer.selectNext();
        if (tokenizer.next.type == TokenType::IDENTIFIER) {
            IdentifierNode* identifierNode = new IdentifierNode();
            identifierNode->create(std::get<std::string>(tokenizer.next.value));
            funcDeclarationNode->addChild(identifierNode);
            tokenizer.selectNext();
            if (tokenizer.next.type == TokenType::OPAR) {
                tokenizer.selectNext();
                if (tokenizer.next.type == TokenType::IDENTIFIER) {
                    VarDeclarationNode* varDeclarationNode = new VarDeclarationNode();
                    funcDeclarationNode->argumentCount++;
                    IdentifierNode* argumentNode = new IdentifierNode();
                    argumentNode->create(std::get<std::string>(tokenizer.next.value));
                    varDeclarationNode->addChild(argumentNode);
                    funcDeclarationNode->addChild(varDeclarationNode);
                    tokenizer.selectNext();
                    while (tokenizer.next.type == TokenType::COMMA) {
                        tokenizer.selectNext();
                        if (tokenizer.next.type == TokenType::IDENTIFIER) {
                            IdentifierNode* newArgumentNode = new IdentifierNode();
                            funcDeclarationNode->argumentCount++;
                            newArgumentNode->create(std::get<std::string>(tokenizer.next.value));
                            varDeclarationNode->addChild(newArgumentNode);
                            tokenizer.selectNext();
                        }
                        else {
                            std::cout << "[ERROR]: Missing argument identifier during function declaration at position " << tokenizer.position - 1 << std::endl;
                            throw 0;
                        }
                    } 
                }
            }
            else {
                std::cout << "[ERROR]: Missing opening parenthesis during function declaration at position" << tokenizer.position - 1 << std::endl;
                throw 0;
            }

            if (tokenizer.next.type == TokenType::CPAR) {
                tokenizer.selectNext(); 
                funcDeclarationNode->addChild(parseBlock());
                return funcDeclarationNode;
            }
            else {
                std::cout << "[ERROR]: Missing closing parenthesis during function declaration at position " << tokenizer.position - 1 << std::endl;
                throw 0;
            }
        }
    }
    else { 
        std::cout << "[ERROR]: Invalid character at position " << tokenizer.position - 1 << std::endl;
        throw 0;
    }
    return new NoOpNode();
}

ASTNode* Parser::parseProgram() {
    ProgramNode* programNode = new ProgramNode();
    while (tokenizer.next.type != TokenType::_EOF) {
        programNode->addChild(parseDeclaration());
    }
    return programNode;
}

