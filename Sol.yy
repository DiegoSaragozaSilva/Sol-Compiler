%{
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>

    int yylex();
    void yyerror(const char* s);

    extern int yylex();
    extern int yyparse();
    extern FILE* yyin;
    extern FILE* yyout;
%}

%token GLOBAL LOCAL ASSIGN OPAR CPAR EQUALS LESS GREATER LESSEQUAL GREATEREQUAL PLUS MINUS OR MUL DIV MOD AND IF ELSE THEN END SWITCH SAE DO BREAK WHILE REPEAT UNTIL IDENTIFIER NUM STRING FUNCTION CASE DEFAULT

%union {
    long value;
    char* stringValue;
};

%type<stringValue> IDENTIFIER STRING

%%

statement: declaration {}
         | assignment {}
         | function_declaration {}
         | loop_statement {}
         | conditional_statement {}
         | statement {};

function_declaration: FUNCTION IDENTIFIER OPAR function_signature_arguments CPAR statement END {}; 

function_signature_arguments: IDENTIFIER {}
                            | IDENTIFIER "," function_signature_arguments {}
                            | "" {};

assignment: IDENTIFIER ASSIGN relational_expression {};

declaration: declaration_flexor IDENTIFIER {}
           | declaration_flexor IDENTIFIER ASSIGN relational_expression {};

declaration_flexor: LOCAL {}
                  | GLOBAL {};

loop_statement: WHILE relational_expression DO statement {}
              | REPEAT statement UNTIL relational_expression {};

conditional_statement: IF relational_expression THEN statement END {}
                     | IF relational_expression THEN statement ELSE statement END {}
                     | SWITCH IDENTIFIER switch_cases {};

switch_cases: CASE NUM DO statement BREAK {}
            | CASE NUM DO statement BREAK switch_cases {}
            | DEFAULT DO statement BREAK {};

factor: NUM {}
      | STRING {}
      | IDENTIFIER {}
      | IDENTIFIER OPAR function_arguments CPAR {}
      | expression_operator factor {}
      | relational_expression {};

function_arguments: relational_expression {}
                  | relational_expression "," function_arguments {}
                  | "" {};

term: factor {}
    | factor term_operator term {};

term_operator: MUL {}
             | DIV {}
             | MOD {}
             | AND {};

expression: term {}
          | term expression_operator expression {};

expression_operator: PLUS {}
                   | MINUS {}
                   | OR {};

relational_expression: expression {}
                     | expression relational_operator relational_expression {};

relational_operator: EQUALS {}
                   | LESS {}
                   | GREATER {}
                   | LESSEQUAL {}
                   | GREATEREQUAL {};
%%

int main() {
    yyin = fopen("in.sol", "r");
    yyout = fopen("out.sol", "w");

    yyparse();

    fclose(yyin);
    fclose(yyout);

    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "%s\n", s);
}

int yywrap() {
    return 0;
}
