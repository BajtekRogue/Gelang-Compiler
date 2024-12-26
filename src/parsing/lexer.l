%{
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <cinttypes>
#include "parser.hpp"

// Store lines for error reporting
std::vector<std::string> inputLines;

void reportLexerError(const std::string& message, size_t lineNumber) {
    std::cerr << Colors::red << "Lexical Error: " << message << " at line " << lineNumber << Colors::reset << std::endl;
    
    if (lineNumber > 0 && lineNumber <= inputLines.size()) {
        std::cerr << Colors::yellow << "Line " << lineNumber << ": " << inputLines[lineNumber - 1] << Colors::reset << std::endl;
    }
    
    exit(1);
}


void initializeLineTracking(std::FILE* inputFile) {
    inputLines.clear();
    
    // Rewind to start of file
    rewind(inputFile);
    
    // Read line by line
    char lineBuffer[1024];
    while (fgets(lineBuffer, sizeof(lineBuffer), inputFile)) {
        // Remove trailing newline if present
        lineBuffer[strcspn(lineBuffer, "\n")] = 0;
        inputLines.push_back(lineBuffer);
    }
    
    // Rewind again for actual parsing
    rewind(inputFile);
}
%}

%option noyywrap
%option yylineno
%option nounput

/* Definitions */
IDENTIFIER  [_a-z]+

%%

[0-9]+      { 
    try{
        yylval.int_val = std::stoll(yytext);  // static cast might be needed her to convert to int64_t
        return NUMBER; 
    }catch (const std::out_of_range&){
        reportLexerError("Number out of range for long long", yylineno);
    }

}

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


 /* Comments */
"#".*           { /* Ignore comments */ }

 /* Whitespace */
[ \t\n]+          { /* Ignore whitespace */ }


. { 
    std::stringstream unexpected;
    unexpected << "Unexpected character '" << yytext[0] << "'";
    reportLexerError(unexpected.str(), yylineno);
}



%%

