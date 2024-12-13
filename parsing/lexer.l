%option c++
%option noyywrap
%option yylineno
%option nounput

%{
#include <iostream>
#include <cstring>
#include "parser.hh"

int lineCounter = 1;
int yylex();
%}

/* Definitions */
DIGIT       [0-9]
IDENTIFIER  [_a-z]+

%%

 /* Keywords */
"PROGRAM"       { return PROGRAM; }
"IS"            { return IS; }
"BEGIN"         { return BEGIN_BLOCK; }
"END"           { return END; }
"IF"            { return IF; }
"THEN"          { return THEN; }
"ELSE"          { return ELSE; }
"ENDIF"         { return ENDIF; }
"WHILE"         { return WHILE; }
"DO"            { return DO; }
"ENDWHILE"      { return ENDWHILE; }
"REPEAT"        { return REPEAT; }
"UNTIL"         { return UNTIL; }
"FOR"           { return FOR; }
"FROM"          { return FROM; }
"TO"            { return TO; }
"DOWNTO"        { return DOWNTO; }
"ENDFOR"        { return ENDFOR; }
"PROCEDURE"     { return PROCEDURE; }
"READ"          { return READ; }
"WRITE"         { return WRITE; }
"T"             { return ARRAY; }

 /* Operators */
":="            { return ASSIGN; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULTIPLY; }
"/"             { return DIVIDE; }
"%"             { return MODULO; }

 /* Comparison Operators */
"="             { return EQ; }
"!="            { return NE; }
"<"             { return LT; }
">"             { return GT; }
"<="            { return LE; }
">="            { return GE; }

 /* Punctuation */
";"             { return SEMICOLON; }
":"             { return COLON; }
","             { return COMMA; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"["             { return LBRACKET; }
"]"             { return RBRACKET; }

 /* Identifiers and Numbers */
{IDENTIFIER}    { 
    yylval.str_val = strdup(yytext); 
    return IDENTIFIER; 
}


[-]?{DIGIT}+        { 
    yylval.int_val = atoi(yytext); 
    return NUMBER; 
}

 /* Comments */
"#".*           { /* Ignore comments */ }

 /* Whitespace */
[ \t]+          { /* Ignore whitespace */ }
\n              { lineCounter++; }



. {
    fprintf(stderr, "\033[1;31mLexical error: Unexpected character '%s' at line number %d\033[0m\n", yytext, lineCounter);
    return ERROR;
}



%%

