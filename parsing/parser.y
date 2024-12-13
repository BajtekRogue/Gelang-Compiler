%code requires{
    #include <iostream>
    #include <vector>
    #include <string>
    #include <memory>
    #include <variant>
    #include <optional>    
}

%{
#include <cstring>
#include "languageStructs.hpp" // Include the C++ structs header

extern "C" int yylineno;
int yylex(void);
void yyerror(Program** parsed_program, const char *s);
void yyset_in(std::FILE * in_str);

%}


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

/* Additional Tokens */
%token ERROR

/* Precedence and Associativity */
%left PLUS MINUS
%left MULTIPLY DIVIDE MODULO
%nonassoc EQ NE LT GT LE GE

/* Types */

%union {
    int int_val;
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
    
    // Variable and parameter types
    struct Variable* variable_val;
    struct Parameter* parameter_val;
    
    // List-like containers
    std::vector<std::unique_ptr<Procedure>>* procedures_vec;
    std::vector<std::unique_ptr<Variable>>* variables_vec;
    std::vector<std::unique_ptr<Command>>* commands_vec;
    std::vector<std::unique_ptr<Parameter>>* parameters_vec;
    std::vector<std::unique_ptr<Value>>* values_vec;
}

%parse-param {Program** parsed_program}

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
%type <value_val> value identifier pidentifier 
%type <int_val>  number

%token <str_val> IDENTIFIER ARRAY
%token <int_val> NUMBER

%%

program_all: procedures main {
    // Allocate memory for parsed_program if not already allocated
    if (*parsed_program == nullptr) {
        *parsed_program = new Program();
    }

    if ($1) {
        (*parsed_program)->procedures = std::move(*$1);
        delete $1;
    }
    
    (*parsed_program)->declarations = std::move($2->declarations);
    (*parsed_program)->mainCommands = std::move($2->commands);
    delete $2;

    $$ = *parsed_program;
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
    | /* empty */ { $$ = nullptr; };

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
        cmd->iterator.reset($2);
        cmd->fromValue.reset($4);
        cmd->toValue.reset($6);
        cmd->commands = std::move(*$8);
        $$ = cmd;
        delete $8;
    }
    | FOR pidentifier FROM value DOWNTO value DO commands ENDFOR {
        auto cmd = new ForDowntoCommand();
        cmd->iterator.reset($2);
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
        proc->identifier = $1->asIdentifier();
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
        cmd->identifier = std::unique_ptr<Value>($1);
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


        if($3->isIdentifier()){
            $1->push_back(std::make_unique<Variable>($3->asIdentifier()));
        }else if($3->isNumber()){
            yyerror(parsed_program, "Number declared as variable");
        }else{
            yyerror(parsed_program, "Array accese declared as variable");
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
        if($3->isIdentifier()){
            $1->push_back(std::make_unique<Variable>($3->asIdentifier(), $5, $7));
        }else if($3->isNumber()){
            yyerror(parsed_program, "Number declared as arrays identifier");
        }else{
            yyerror(parsed_program, "Array accese declared as array identifier");
        }
        $$ = $1;
        
        if ($3){
            free($3);
        }
    }
    | pidentifier {
        $$ = new std::vector<std::unique_ptr<Variable>>();
        if($1->isIdentifier()){
            $$->push_back(std::make_unique<Variable>($1->asIdentifier()));
        }else if($1->isNumber()){
            yyerror(parsed_program, "Number declared as variable");
        }else{
            yyerror(parsed_program, "Array accese declared as variable");
        }
        
        if ($1){
            free($1);
        }
    }
    | pidentifier LBRACKET NUMBER COLON NUMBER RBRACKET {
        $$ = new std::vector<std::unique_ptr<Variable>>();

        if($1->isIdentifier()){
            $$->push_back(std::make_unique<Variable>($1->asIdentifier(), $3, $5));
        }else if($1->isNumber()){
            yyerror(parsed_program, "Number declared as arrays identifier");
        }else{
            yyerror(parsed_program, "Array accese declared as array identifier");
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
        if($3->isIdentifier()){
            $1->push_back(std::make_unique<Parameter>($3->asIdentifier()));
        }else if($3->isNumber()){
            yyerror(parsed_program, "Number declared as procedure parameter");
        }else{
            yyerror(parsed_program, "Array accese declared as procedure parameter");
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
        if($4->isIdentifier()){
            $1->push_back(std::make_unique<Parameter>($4->asIdentifier(), true));
        }else if($4->isNumber()){
            yyerror(parsed_program, "Number declared as procedure parameter");
        }else{
            yyerror(parsed_program, "Array accese declared as procedure parameter");
        } 
        $$ = $1;
        if ($4) {
            free($4);
        }
    }
    | pidentifier {
        $$ = new std::vector<std::unique_ptr<Parameter>>();        
        if($1->isIdentifier()){
            $$->push_back(std::make_unique<Parameter>($1->asIdentifier()));
        }else if($1->isNumber()){
            yyerror(parsed_program, "Number declared as procedure parameter");
        }else{
            yyerror(parsed_program, "Array accese declared as procedure parameter");
        } 
        if ($1) {
            free($1);
        }
    }
    | ARRAY pidentifier {
        $$ = new std::vector<std::unique_ptr<Parameter>>();
        if($2->isIdentifier()){
            $$->push_back(std::make_unique<Parameter>($2->asIdentifier(), true));
        }else if($2->isNumber()){
            yyerror(parsed_program, "Number declared as procedure parameter");
        }else{
            yyerror(parsed_program, "Array accese declared as procedure parameter");
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
        if($3->isIdentifier()){
            $1->push_back(std::make_unique<Value>($3->asIdentifier()));
        }else if($3->isNumber()){
            $1->push_back(std::make_unique<Value>($3->asNumber()));
        }else{
            $1->push_back(std::make_unique<Value>($3->asArrayAccess()));
        }         
        $$ = $1;
        if ($3) {
            free($3);
        }
    }
    | pidentifier {
        $$ = new std::vector<std::unique_ptr<Value>>();
        if($1->isIdentifier()){
            $$->push_back(std::make_unique<Value>($1->asIdentifier()));
        }else if($1->isNumber()){
            $$->push_back(std::make_unique<Value>($1->asNumber()));
        }else{
            $$->push_back(std::make_unique<Value>($1->asArrayAccess()));
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
        $$ = $1;
    };

pidentifier: 
    IDENTIFIER {
        $$ = new Value(strdup($1));
    };

number: 
    NUMBER {
        $$ = $1;
    };

identifier: 
    pidentifier {
        $$ = $1;
    }
    | pidentifier LBRACKET pidentifier RBRACKET {
        auto arr_access = new ArrayAccess($1->asIdentifier(), $3->asIdentifier());
        $$ = new Value(*arr_access);
        delete arr_access;
    }
    | pidentifier LBRACKET NUMBER RBRACKET {
        auto arr_access = new ArrayAccess($1->asIdentifier(), $3);
        $$ = new Value(*arr_access);
        delete arr_access;
};


%%

/* Error Handling */
void yyerror(Program** parsed_program, const char* s) {
    fprintf(stderr, "\033[1;31mSyntax error: %s at line number %d\033[0m\n", s, yylineno);

    // Clean up allocated memory
    delete *parsed_program;
    *parsed_program = nullptr;

    exit(1); // Stop parsing on error
}


void run_parser(std::FILE* input, std::unique_ptr<Program>& parsed_program) {
    std::cout << "\033[1;34mParsing code.\033[0m" << std::endl;
    Program* program = nullptr;
    yyset_in(input);
    yyparse(&program);
    parsed_program.reset(program); // Transfer ownership to unique_ptr
    std::cout << "\033[1;34mFinished parsing code.\033[0m" << std::endl;
}
