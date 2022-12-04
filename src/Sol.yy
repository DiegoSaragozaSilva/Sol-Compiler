%{
    #include "Node.hpp"

    ProgramNode* programNode;
    
    extern int yylineno;
    extern int yylex();
    void yyerror(const char* error) { std::cout << "[ERROR]: " << error << " in line " << yylineno << std::endl; }
%}

%define parse.error verbose

%union {
    ASTNode* node;
    std::vector<ASTNode*> *nodes;
    std::string* string;
    int token;
};

%token <string> IDENTIFIER STRING NUM
%token <token> GLOBAL LOCAL ASSIGN OPAR CPAR EQUALS LESS GREATER LESSEQUAL GREATEREQUAL PLUS MINUS OR MUL DIV MOD AND IF ELSE THEN END SWITCH SAE DO BREAK WHILE REPEAT UNTIL FUNCTION CASE DEFAULT VAR _BEGIN COMMA

%type <node> relational_operator expression_operator term_operator factor block assignment loop_statement conditional_statement function_declaration program variable_declaration relational_expression term expression
%type <nodes> declaration statement function_signature_arguments function_arguments

%left PLUS MINUS 
%left MUL DIV MOD

%start program

%%

program: declaration { 
        programNode = new ProgramNode();
        for (ASTNode* node : *$1)
            programNode->addChild(node);
       }
       ;

declaration: function_declaration {$$ = new NodeList(); $$->push_back($1);}
           | function_declaration declaration {$2->push_back($1); $$ = $2;}
           ;

block: _BEGIN statement END {
        $$ = new BlockNode();
        for (ASTNode* node : *$2) 
            $$->addChild(node);
     }
     ;

statement: {$$ = new NodeList(); }
         | assignment statement {$2->push_back($1); $$ = $2;}
         | variable_declaration statement {$2->push_back($1); $$ = $2;}
         | loop_statement statement {$2->push_back($1); $$ = $2;}
         | conditional_statement statement {$2->push_back($1); $$ = $2;}
         | block statement {$2->push_back($1); $$ = $2;}
         ;

function_declaration: FUNCTION IDENTIFIER OPAR function_signature_arguments CPAR block {
                        FuncDeclarationNode* funcDeclarationNode = new FuncDeclarationNode();
                        IdentifierNode* identifierNode = new IdentifierNode();
                        identifierNode->create(*$2);
                        funcDeclarationNode->addChild(identifierNode);
                        for (ASTNode* node : *$4)
                            funcDeclarationNode->addChild(node);
                        funcDeclarationNode->addChild($6);
                        funcDeclarationNode->argumentCount = $4->size();
                        $$ = funcDeclarationNode;
                    }
                    ; 

function_signature_arguments: {$$ = new NodeList();}
                            | IDENTIFIER {
                                IdentifierNode* identifierNode = new IdentifierNode();
                                identifierNode->create(*$1);
                                $$->push_back(identifierNode);
                            }
                            | IDENTIFIER COMMA function_signature_arguments {
                                IdentifierNode* identifierNode = new IdentifierNode();
                                identifierNode->create(*$1);
                                $3->push_back(identifierNode);
                                $$ = $3;
                            }
                            ;

assignment: IDENTIFIER ASSIGN relational_expression {
            $$ = new AssignmentNode();
            IdentifierNode* identifierNode = new IdentifierNode();
            identifierNode->create(*$1);
            $$->addChild(identifierNode);
            $$->addChild($3);
          }
          ;

variable_declaration: VAR IDENTIFIER {
                        $$ = new VarDeclarationNode();
                        IdentifierNode* identifierNode = new IdentifierNode();
                        identifierNode->create(*$2);
                        $$->addChild(identifierNode);
                        $$->type = DataType::I32_T; 
                    }
                    ;

//declaration: declaration_flexor IDENTIFIER {}
//           | declaration_flexor IDENTIFIER ASSIGN relational_expression {};

// declaration_flexor: LOCAL {}
//                   | GLOBAL {};

loop_statement: WHILE relational_expression block {
                $$ = new LoopNode();
                $$->create("while");
                $$->addChild($2);
                $$->addChild($3);
              }
              ;
//            | REPEAT statement UNTIL relational_expression {};

conditional_statement: IF relational_expression block {
                        $$ = new ConditionalNode();
                        $$->create("if");
                        $$->addChild($2);
                        $$->addChild($3);
                     }
                     | IF relational_expression block ELSE block {
                        $$ = new ConditionalNode();
                        $$->create("if");
                        $$->addChild($2);
                        $$->addChild($3);
                        $$->addChild($5);
                     }
                     ;
//                   | SWITCH IDENTIFIER switch_cases {};

/*switch_cases: CASE NUM DO statement BREAK {}*/
/*            | CASE NUM DO statement BREAK switch_cases {}*/
/*            | DEFAULT DO statement BREAK {};*/

factor: NUM {$$ = new IntValNode(); $$->create(stoi(*$1));}
      | STRING {$$ = new StrValNode(); $$->create(*$1);}
      | IDENTIFIER {$$ = new IdentifierNode(); $$->create(*$1);}
      | IDENTIFIER OPAR function_arguments CPAR {
        $$ = new FuncCallNode();
        IdentifierNode* identifierNode = new IdentifierNode();
        identifierNode->create(*$1);
        $$->addChild(identifierNode);
        for (ASTNode* node : *$3)
            $$->addChild(node);
      }
      | OPAR relational_expression CPAR {$$ = $2;}
      ;

function_arguments: relational_expression {$$->push_back($1);}
                  | relational_expression COMMA function_arguments {$3->push_back($1); $$ = $3;}
                  | {$$ = new NodeList();};

term: factor {}
    | factor term_operator term {
        $2->addChild($1);
        $2->addChild($3);
    }
    ;

term_operator: MUL {$$ = new BinOpNode(); $$->create("*");}
             | DIV {$$ = new BinOpNode(); $$->create("/");}
             | MOD {$$ = new BinOpNode(); $$->create("%");}
             | AND {$$ = new BinOpNode(); $$->create("&&");};

expression: term {}
          | term expression_operator expression {
            $2->addChild($1);
            $2->addChild($3);
          }
          ;

expression_operator: PLUS {$$ = new BinOpNode(); $$->create("+");}
                   | MINUS {$$ = new BinOpNode(); $$->create("-");}
                   | OR {$$ = new BinOpNode(); $$->create("||");};

relational_expression: expression {}
                     | expression relational_operator relational_expression {
                        $2->addChild($1);
                        $2->addChild($3);
                     }
                     ;

relational_operator: EQUALS {$$ = new BinOpNode(); $$->create("==");}
                   | LESS {$$ = new BinOpNode(); $$->create("<");}
                   | GREATER {$$ = new BinOpNode(); $$->create(">");}
                   | LESSEQUAL {$$ = new BinOpNode(); $$->create("<=");}
                   | GREATEREQUAL {$$ = new BinOpNode(); $$->create(">=");}
                   ;
%%
