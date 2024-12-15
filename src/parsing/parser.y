%code requires {
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <variant>
#include <optional>   
#include <cstring>
#include "../headers/languageStructs.hpp"
#include "../headers/colors.hpp"
}
%{
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <variant>
#include <optional>   
#include <cstring>
#include "../headers/languageStructs.hpp"
#include "../headers/colors.hpp" //colors for printing

extern int yylineno;
extern int yylex(void);
extern void initializeLineTracking(std::FILE* inputFile);
extern std::vector<std::string> inputLines;
void yyerror(Program** parsedProgram, const char *s);
void yyset_in(std::FILE * inStr);

%}

%locations
%define parse.error verbose
%parse-param {Program** parsedProgram}


/* Full Token Definitions */
%token PROGRAM IS BEGIN_BLOCK END 
%token IF THEN ELSE ENDIF 
%token WHILE DO ENDWHILE 
%token REPEAT UNTIL 
%token FOR FROM TO DOWNTO ENDFOR
%token PROCEDURE 
%token READ WRITE

/* Arithmetic and Assignment */
%token ASSIGN
%token PLUS MINUS MULTIPLY DIVIDE MODULO

/* Comparison Operators */
%token EQ NE LT GT LE GE

/* Punctuation Tokens */
%token SEMICOLON COLON COMMA 
%token LPAREN RPAREN 
%token LBRACKET RBRACKET


/* Types */
%union {
    long long int_val;
    char* str_val;
    
    // Program types
    struct Program* program_val;
    struct Procedure* procedure_val;
    struct Main* main_val;
    
    // Command types
    struct Command* command_val;
    struct ProcedureCallCommand* proc_call_command_val;

    // Expression and condition types
    struct Expression* expression_val;
    struct Condition* condition_val;
    
    // Value types
    struct Value* value_val;
    struct Identifier* identifier_val;
    struct Variable* variable_val;
    struct Parameter* parameter_val;
    
    // List-like containers
    std::vector<std::unique_ptr<Procedure>>* procedures_vec;
    std::vector<std::unique_ptr<Variable>>* variables_vec;
    std::vector<std::unique_ptr<Command>>* commands_vec;
    std::vector<std::unique_ptr<Parameter>>* parameters_vec;
    std::vector<std::unique_ptr<Value>>* values_vec;
}


%type <program_val> program_all
%type <procedures_vec> procedures
%type <main_val> main
%type <commands_vec> commands 
%type <command_val> command
%type <variables_vec> declarations
%type <procedure_val> proc_head
%type <parameters_vec> args_decl 
%type <values_vec> args
%type <proc_call_command_val> proc_call
%type <expression_val> expression
%type <condition_val> condition
%type <value_val> value 
%type <identifier_val> identifier 
%type <str_val> pidentifier 
%type <int_val>  number

%token <str_val> IDENTIFIER ARRAY
%token <int_val> NUMBER

%%

program_all: procedures main {
    if (*parsedProgram == nullptr) {
        *parsedProgram = new Program();
    }
    if ($1) {
        (*parsedProgram)->procedures = std::move(*$1);
        delete $1;
    }
    (*parsedProgram)->declarations = std::move($2->declarations);
    (*parsedProgram)->mainCommands = std::move($2->commands);
    $$ = *parsedProgram;
    delete $2;
};


procedures: 
    procedures PROCEDURE proc_head IS declarations BEGIN_BLOCK commands END {
        if (!$1){
            $1 = new std::vector<std::unique_ptr<Procedure>>();
        }
        $1->push_back(std::unique_ptr<Procedure>($3));
        $3->declarations = std::move(*$5);
        $3->commands = std::move(*$7);
        $$ = $1;
        delete $5;
        delete $7;
    }
    | procedures PROCEDURE proc_head IS BEGIN_BLOCK commands END {
        if (!$1){
            $1 = new std::vector<std::unique_ptr<Procedure>>();
        }
        $1->push_back(std::unique_ptr<Procedure>($3));
        $3->commands = std::move(*$6);
        $$ = $1;
        delete $6;
    }
    | %empty { $$ = nullptr; };

main: 
    PROGRAM IS declarations BEGIN_BLOCK commands END {
        auto main = std::make_unique<Main>();
        main->declarations = std::move(*$3);
        main->commands = std::move(*$5);
        $$ = main.release();
        delete $3;
        delete $5;
    }
    | PROGRAM IS BEGIN_BLOCK commands END {
        auto main = std::make_unique<Main>();
        main->commands = std::move(*$4);
        $$ = main.release();
        delete $4;
    };

commands: 
    commands command {
        if (!$1) {
            $1 = new std::vector<std::unique_ptr<Command>>();
        }
        $1->push_back(std::unique_ptr<Command>(static_cast<Command*>($2))); 
        $$ = $1;
    }
    | command {
        $$ = new std::vector<std::unique_ptr<Command>>();
        $$->push_back(std::unique_ptr<Command>(static_cast<Command*>($1)));  
    };

command: 
    identifier ASSIGN expression SEMICOLON {
        auto cmd = new AssignCommand();
        cmd->identifier.reset($1);
        cmd->expression.reset($3);
        $$ = cmd;
    }
    | IF condition THEN commands ELSE commands ENDIF {
        auto cmd = new IfElseCommand();
        cmd->condition.reset($2);
        cmd->thenCommands = std::move(*$4);
        cmd->elseCommands = std::move(*$6);
        $$ = cmd;
        delete $4;
        delete $6;
    }
    | IF condition THEN commands ENDIF {
        auto cmd = new IfCommand();
        cmd->condition.reset($2);
        cmd->thenCommands = std::move(*$4);
        $$ = cmd;
        delete $4;
    }
    | WHILE condition DO commands ENDWHILE {
        auto cmd = new WhileCommand();
        cmd->condition.reset($2);
        cmd->commands = std::move(*$4);
        $$ = cmd;
        delete $4;
    }
    | REPEAT commands UNTIL condition SEMICOLON {
        auto cmd = new RepeatCommand();
        cmd->commands = std::move(*$2);
        cmd->condition.reset($4);
        $$ = cmd;
        delete $2;
    }
    | FOR pidentifier FROM value TO value DO commands ENDFOR {
        auto cmd = new ForToCommand();
        cmd->iterator = std::string($2);
        cmd->fromValue.reset($4);
        cmd->toValue.reset($6);
        cmd->commands = std::move(*$8);
        $$ = cmd;
        delete $8;
    }
    | FOR pidentifier FROM value DOWNTO value DO commands ENDFOR {
        auto cmd = new ForDowntoCommand();
        cmd->iterator = std::string($2);
        cmd->fromValue.reset($4);
        cmd->downtoValue.reset($6);
        cmd->commands = std::move(*$8);
        $$ = cmd;
        delete $8;
    }
    | proc_call SEMICOLON {
        $$ = $1;
    }
    | READ identifier SEMICOLON {
        auto cmd = new ReadCommand();
        cmd->identifier.reset($2);
        $$ = cmd;
    }
    | WRITE value SEMICOLON {
        auto cmd = new WriteCommand();
        cmd->value.reset($2);
        $$ = cmd;
    };

proc_head: 
    pidentifier LPAREN args_decl RPAREN {
        auto proc = new Procedure();
        proc->identifier = std::string($1);
        proc->parameters = $3 ? std::move(*$3) : std::vector<std::unique_ptr<Parameter>>();
        $$ = proc;
        if ($1) {
            free($1);
        }
        if ($3) {
            delete $3;
        }
    };

proc_call: 
    pidentifier LPAREN args RPAREN {
        auto cmd = new ProcedureCallCommand();
        cmd->identifier = std::string($1);
        cmd->arguments = $3 ? std::move(*$3) : std::vector<std::unique_ptr<Value>>();
        $$ = cmd;
        if ($1) {
            free($1);
        }
        if ($3) {
            delete $3;
        }
    };

declarations: 
    declarations COMMA pidentifier {
        if (!$1){
            $1 = new std::vector<std::unique_ptr<Variable>>();
        }
        if($3){
            $1->push_back(std::make_unique<Variable>(std::string($3)));
        }
        $$ = $1;
        if ($3){
            free($3);
        }
    }
    | declarations COMMA pidentifier LBRACKET NUMBER COLON NUMBER RBRACKET {
        if (!$1) {
            $1 = new std::vector<std::unique_ptr<Variable>>();
        }
        if($3){
            $1->push_back(std::make_unique<Variable>(std::string($3), $5, $7));
        }
        $$ = $1;
        if ($3){
            free($3);
        }
    }
    | pidentifier {
        $$ = new std::vector<std::unique_ptr<Variable>>();
        if($1){
            $$->push_back(std::make_unique<Variable>(std::string($1)));
        }
        if ($1){
            free($1);
        }
    }
    | pidentifier LBRACKET NUMBER COLON NUMBER RBRACKET {
        $$ = new std::vector<std::unique_ptr<Variable>>();
        if($1){
            $$->push_back(std::make_unique<Variable>(std::string($1), $3, $5));
        }
        if ($1){
            free($1);
        }
    };

args_decl: 
    args_decl COMMA pidentifier {
        if (!$1){
            $1 = new std::vector<std::unique_ptr<Parameter>>();
        }
        if($3){
            $1->push_back(std::make_unique<Parameter>(std::string($3)));
        }
        $$ = $1;
        if ($3) {
            free($3);
        }
    }
    | args_decl COMMA ARRAY pidentifier {
        if (!$1){
            $1 = new std::vector<std::unique_ptr<Parameter>>();
        }        
        if($4){
            $1->push_back(std::make_unique<Parameter>(std::string($4), true));
        }
        $$ = $1;
        if ($4) {
            free($4);
        }
    }
    | pidentifier {
        $$ = new std::vector<std::unique_ptr<Parameter>>();        
        if($1){
            $$->push_back(std::make_unique<Parameter>(std::string($1)));
        }
        if ($1) {
            free($1);
        }
    }
    | ARRAY pidentifier {
        $$ = new std::vector<std::unique_ptr<Parameter>>();
        if($2){
            $$->push_back(std::make_unique<Parameter>(std::string($2), true));
        }
        if ($2) {
            free($2);
        }
    };

args: 
    args COMMA pidentifier {
        if (!$1){
            $1 = new std::vector<std::unique_ptr<Value>>();
        }
        if($3){
            $1->push_back(std::make_unique<Value>(std::string($3)));
        }     
        $$ = $1;
        if ($3) {
            free($3);
        }
    }
    | pidentifier {
        $$ = new std::vector<std::unique_ptr<Value>>();
        if($1){
            $$->push_back(std::make_unique<Value>(std::string($1)));
        }
        if ($1) {
            free($1);
        }
    };

expression: 
    value {
        auto expr = new Expression();
        expr->type = ExpressionType::Value;
        expr->left.reset($1);
        $$ = expr;
    }
    | value PLUS value {
        auto expr = new Expression();
        expr->type = ExpressionType::Plus;
        expr->left.reset($1);
        expr->right.reset($3);
        $$ = expr;
    }
    | value MINUS value {
        auto expr = new Expression();
        expr->type = ExpressionType::Minus;
        expr->left.reset($1);
        expr->right.reset($3);
        $$ = expr;
    }
    | value MULTIPLY value {
        auto expr = new Expression();
        expr->type = ExpressionType::Multiply;
        expr->left.reset($1);
        expr->right.reset($3);
        $$ = expr;
    }
    | value DIVIDE value {
        auto expr = new Expression();
        expr->type = ExpressionType::Divide;
        expr->left.reset($1);
        expr->right.reset($3);
        $$ = expr;
    }
    | value MODULO value {
        auto expr = new Expression();
        expr->type = ExpressionType::Modulo;
        expr->left.reset($1);
        expr->right.reset($3);
        $$ = expr;
    };

condition: 
    value EQ value {
        auto cond = new Condition();
        cond->type = ConditionType::Equal;
        cond->left.reset($1);
        cond->right.reset($3);
        $$ = cond;
    }
    | value NE value {
        auto cond = new Condition();
        cond->type = ConditionType::NotEqual;
        cond->left.reset($1);
        cond->right.reset($3);
        $$ = cond;
    }
    | value GT value {
        auto cond = new Condition();
        cond->type = ConditionType::GreaterThan;
        cond->left.reset($1);
        cond->right.reset($3);
        $$ = cond;
    }
    | value LT value {
        auto cond = new Condition();
        cond->type = ConditionType::LessThan;
        cond->left.reset($1);
        cond->right.reset($3);
        $$ = cond;
    }
    | value GE value {
        auto cond = new Condition();
        cond->type = ConditionType::GreaterEqual;
        cond->left.reset($1);
        cond->right.reset($3);
        $$ = cond;
    }
    | value LE value {
        auto cond = new Condition();
        cond->type = ConditionType::LessEqual;
        cond->left.reset($1);
        cond->right.reset($3);
        $$ = cond;
    };

value: 
    number {
        $$ = new Value($1);
    }
    | identifier {
        $$ = new Value(std::move(*$1));
    };

pidentifier: 
    IDENTIFIER {
        $$ = strdup($1);
    };

number: 
    NUMBER {
        $$ = $1;
    };

identifier: 
    pidentifier {
        $$ = new Identifier(std::string($1));
    }
    | pidentifier LBRACKET pidentifier RBRACKET {
        auto arr_access = new ArrayAccess(std::string($1), std::string($3));
        $$ = new Identifier(std::string($1), *arr_access);
        delete arr_access;
    }
    | pidentifier LBRACKET NUMBER RBRACKET {
        auto arr_access = new ArrayAccess(std::string($1), $3);
        $$ = new Identifier(std::string($1), *arr_access);
        delete arr_access;
};


%%

void yyerror(Program** parsedProgram, const char* s) {
    extern char* yytext;  // The current token text
    extern int yylineno;  // The current line number

    // Print the standard error message
    fprintf(stderr, "%sSyntax Error: Unexpected token: '%s' at line %d %s\n", 
            color_Red.c_str(), yytext ? yytext : "unknown", yylineno, color_Reset.c_str());

    if (yylineno > 0 && static_cast<size_t>(yylineno) <= inputLines.size()) {
        fprintf(stderr, "%sLine %d: %s%s\n", 
            color_Yellow.c_str(), yylineno, inputLines[static_cast<size_t>(yylineno) - 1].c_str(), color_Reset.c_str());
    }

    // Clean up the program if partially parsed
    if (*parsedProgram) {
        delete *parsedProgram;
        *parsedProgram = nullptr;
    }

    // Stop parsing
    exit(1);
}

void runParser(std::FILE* input, std::unique_ptr<Program>& parsedProgram) {
    std::cout << color_Blue << "Parsing code..." <<color_Reset << std::endl;
    Program* program = nullptr;
    
    // Reset input file pointer to beginning
    std::rewind(input);
    
    // Set input file for lexer
    yyset_in(input);
    initializeLineTracking(input);

    int parseResult = yyparse(&program);
    
    if (parseResult != 0 || program == nullptr) {
        std::cerr << color_Red << "Parsing failed!" << color_Reset << std::endl;
        if (program){
            delete program;
        }
        return;
    }
    
    parsedProgram.reset(program); 
    std::cout << color_Cyan << "Finished parsing code." << color_Reset << std::endl;
}
