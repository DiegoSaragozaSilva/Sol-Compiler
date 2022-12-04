%{
    #include <string>

    #include "Node.hpp"
    #include "SolParser.hpp"

    #define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
    #define TOKEN(t) (yylval.token = t)
%}

%option noyywrap yylineno

NUM             [0-9][0-9]*
STRING          [a-zA-Z0-9_]+
IDENTIFIER      [a-zA-Z][a-zA-Z0-9_]* 

%%

[ \t\n]     ;
"global"    return TOKEN(GLOBAL);
"local"     return TOKEN(LOCAL);
"="         return TOKEN(ASSIGN);
"("         return TOKEN(OPAR);
")"         return TOKEN(CPAR);
"=="        return TOKEN(EQUALS);
"<"         return TOKEN(LESS);
">"         return TOKEN(GREATER);
"<="        return TOKEN(LESSEQUAL);
">="        return TOKEN(GREATEREQUAL);
"+"         return TOKEN(PLUS);
"-"         return TOKEN(MINUS);
"or"        return TOKEN(OR);
"*"         return TOKEN(MUL);
"/"         return TOKEN(DIV);
"%"         return TOKEN(MOD);
"and"       return TOKEN(AND);
"if"        return TOKEN(IF);
"else"      return TOKEN(ELSE);
"then"      return TOKEN(THEN);
"end"       return TOKEN(END);
"switch"    return TOKEN(SWITCH);
"case"      return TOKEN(CASE);
"do"        return TOKEN(DO);
"break"     return TOKEN(BREAK);
"while"     return TOKEN(WHILE);
"repeat"    return TOKEN(REPEAT);
"until"     return TOKEN(UNTIL);
"function"  return TOKEN(FUNCTION);
"default"   return TOKEN(DEFAULT);
"var"       return TOKEN(VAR);
"begin"     return TOKEN(_BEGIN);
","         return TOKEN(COMMA);
{IDENTIFIER}        {SAVE_TOKEN; return IDENTIFIER;}
{NUM}               {SAVE_TOKEN; return NUM;}
{STRING}            {SAVE_TOKEN; return STRING;}
.                   {printf("\nUnkown character %s\n", yylval.string); yyterminate();} 

%%
