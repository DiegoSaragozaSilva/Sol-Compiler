%{
    #include <stdio.h>
    #include <string.h>

    #include "Sol.tab.hh"

    extern char* strdup(const char*);
%}

NUM             [0-9][0-9]*
STRING          [a-zA-Z0-9_]+
IDENTIFIER      [a-zA-Z][a-zA-Z0-9_]* 

%%

"global"    return(GLOBAL);
"local"     return(LOCAL);
"="         return(ASSIGN);
"("         return(OPAR);
")"         return(CPAR);
"=="        return(EQUALS);
"<"         return(LESS);
">"         return(GREATER);
"<="        return(LESSEQUAL);
">="        return(GREATEREQUAL);
"+"         return(PLUS);
"-"         return(MINUS);
"or"        return(OR);
"*"         return(MUL);
"/"         return(DIV);
"%"         return(MOD);
"and"       return(AND);
"if"        return(IF);
"else"      return(ELSE);
"then"      return(THEN);
"end"       return(END);
"switch"    return(SWITCH);
"case"      return(CASE);
"do"        return(DO);
"break"     return(BREAK);
"while"     return(WHILE);
"repeat"    return(REPEAT);
"until"     return(UNTIL);
"function"  return(FUNCTION);
"default"   return(DEFAULT);
{IDENTIFIER}        {yylval.stringValue = strdup(yytext); return(IDENTIFIER);}
{NUM}               {yylval.value = atoi(yytext); return(NUM);}
{STRING}            {yylval.stringValue = strdup(yytext); return(STRING);}
.           {printf("\nUnkown character %s\n", yylval.stringValue);} 

%%
