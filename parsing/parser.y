%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

extern int yylineno;
extern int yylex();
void yyerror(const char *s);

// Global JSON object to store the entire program structure
struct json_object *program_json;
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
    char *str_val;
    struct json_object *json_obj;
}

%type <json_obj> program_all procedures main commands command 
%type <json_obj> proc_head proc_call declarations args_decl args 
%type <json_obj> expression condition value identifier pidentifier number

%token <str_val> IDENTIFIER
%token <int_val> NUMBER

%%

/* Grammar Rules */
program_all: procedures main {
    program_json = json_object_new_object();
    json_object_object_add(program_json, "procedures", $1 ? $1 : json_object_new_null());
    json_object_object_add(program_json, "main", $2);
    $$ = program_json;
};

procedures: procedures PROCEDURE proc_head IS declarations BEGIN_BLOCK commands END {
    if (!$1) $1 = json_object_new_array();
    struct json_object *proc_obj = json_object_new_object();
    json_object_object_add(proc_obj, "head", $3);
    json_object_object_add(proc_obj, "declarations", $5);
    json_object_object_add(proc_obj, "commands", $7);
    json_object_array_add($1, proc_obj);
    $$ = $1;
}
| procedures PROCEDURE proc_head IS BEGIN_BLOCK commands END {
    if (!$1) $1 = json_object_new_array();
    struct json_object *proc_obj = json_object_new_object();
    json_object_object_add(proc_obj, "head", $3);
    json_object_object_add(proc_obj, "commands", $6);
    json_object_array_add($1, proc_obj);
    $$ = $1;
}
| /* empty */ { $$ = NULL; };

main: PROGRAM IS declarations BEGIN_BLOCK commands END {
    struct json_object *main_obj = json_object_new_object();
    json_object_object_add(main_obj, "declarations", $3);
    json_object_object_add(main_obj, "commands", $5);
    $$ = main_obj;
}
| PROGRAM IS BEGIN_BLOCK commands END {
    struct json_object *main_obj = json_object_new_object();
    json_object_object_add(main_obj, "commands", $4);
    $$ = main_obj;
};

commands: commands command {
    if (!$1) $1 = json_object_new_array();
    json_object_array_add($1, $2);
    $$ = $1;
}
| command {
    struct json_object *cmd_array = json_object_new_array();
    json_object_array_add(cmd_array, $1);
    $$ = cmd_array;
};

command: identifier ASSIGN expression SEMICOLON {
    struct json_object *cmd_obj = json_object_new_object();
    json_object_object_add(cmd_obj, "type", json_object_new_string("assign"));
    json_object_object_add(cmd_obj, "identifier", $1);
    json_object_object_add(cmd_obj, "expression", $3);
    $$ = cmd_obj;
}
| IF condition THEN commands ELSE commands ENDIF {
    struct json_object *cmd_obj = json_object_new_object();
    json_object_object_add(cmd_obj, "type", json_object_new_string("if_else"));
    json_object_object_add(cmd_obj, "condition", $2);
    json_object_object_add(cmd_obj, "then_commands", $4);
    json_object_object_add(cmd_obj, "else_commands", $6);
    $$ = cmd_obj;
}
| IF condition THEN commands ENDIF {
    struct json_object *cmd_obj = json_object_new_object();
    json_object_object_add(cmd_obj, "type", json_object_new_string("if"));
    json_object_object_add(cmd_obj, "condition", $2);
    json_object_object_add(cmd_obj, "commands", $4);
    $$ = cmd_obj;
}
| WHILE condition DO commands ENDWHILE {
    struct json_object *cmd_obj = json_object_new_object();
    json_object_object_add(cmd_obj, "type", json_object_new_string("while"));
    json_object_object_add(cmd_obj, "condition", $2);
    json_object_object_add(cmd_obj, "commands", $4);
    $$ = cmd_obj;
}
| REPEAT commands UNTIL condition SEMICOLON {
    struct json_object *cmd_obj = json_object_new_object();
    json_object_object_add(cmd_obj, "type", json_object_new_string("repeat"));
    json_object_object_add(cmd_obj, "commands", $2);
    json_object_object_add(cmd_obj, "condition", $4);
    $$ = cmd_obj;
}
| FOR pidentifier FROM value TO value DO commands ENDFOR {
    struct json_object *cmd_obj = json_object_new_object();
    json_object_object_add(cmd_obj, "type", json_object_new_string("for_to"));
    json_object_object_add(cmd_obj, "identifier", $2);
    json_object_object_add(cmd_obj, "from", $4);
    json_object_object_add(cmd_obj, "to", $6);
    json_object_object_add(cmd_obj, "commands", $8);
    $$ = cmd_obj;
}
| FOR pidentifier FROM value DOWNTO value DO commands ENDFOR {
    struct json_object *cmd_obj = json_object_new_object();
    json_object_object_add(cmd_obj, "type", json_object_new_string("for_downto"));
    json_object_object_add(cmd_obj, "identifier", $2);
    json_object_object_add(cmd_obj, "from", $4);
    json_object_object_add(cmd_obj, "downto", $6);
    json_object_object_add(cmd_obj, "commands", $8);
    $$ = cmd_obj;
}
| proc_call SEMICOLON { $$ = $1; }
| READ identifier SEMICOLON {
    struct json_object *cmd_obj = json_object_new_object();
    json_object_object_add(cmd_obj, "type", json_object_new_string("read"));
    json_object_object_add(cmd_obj, "identifier", $2);
    $$ = cmd_obj;
}
| WRITE value SEMICOLON {
    struct json_object *cmd_obj = json_object_new_object();
    json_object_object_add(cmd_obj, "type", json_object_new_string("write"));
    json_object_object_add(cmd_obj, "value", $2);
    $$ = cmd_obj;
};

proc_head: pidentifier LPAREN args_decl RPAREN {
    struct json_object *head_obj = json_object_new_object();
    json_object_object_add(head_obj, "identifier", $1);
    json_object_object_add(head_obj, "arguments", $3 ? $3 : json_object_new_null());
    $$ = head_obj;
};

proc_call: pidentifier LPAREN args RPAREN {
    struct json_object *call_obj = json_object_new_object();
    json_object_object_add(call_obj, "type", json_object_new_string("procedure_call"));
    json_object_object_add(call_obj, "identifier", $1);
    json_object_object_add(call_obj, "arguments", $3 ? $3 : json_object_new_null());
    $$ = call_obj;
};

declarations: declarations COMMA pidentifier {
    if (!$1) $1 = json_object_new_array();
    json_object_array_add($1, $3);
    $$ = $1;
}
| declarations COMMA pidentifier LBRACKET NUMBER COLON NUMBER RBRACKET {
    if (!$1) $1 = json_object_new_array();
    struct json_object *arr_decl = json_object_new_object();
    json_object_object_add(arr_decl, "identifier", $3);
    json_object_object_add(arr_decl, "start_index", json_object_new_int($5));
    json_object_object_add(arr_decl, "end_index", json_object_new_int($7));
    json_object_array_add($1, arr_decl);
    $$ = $1;
}
| pidentifier {
    struct json_object *decl_array = json_object_new_array();
    json_object_array_add(decl_array, $1);
    $$ = decl_array;
}
| pidentifier LBRACKET NUMBER COLON NUMBER RBRACKET {
    struct json_object *decl_array = json_object_new_array();
    struct json_object *arr_decl = json_object_new_object();
    json_object_object_add(arr_decl, "identifier", $1);
    json_object_object_add(arr_decl, "start_index", json_object_new_int($3));
    json_object_object_add(arr_decl, "end_index", json_object_new_int($5));
    json_object_array_add(decl_array, arr_decl);
    $$ = decl_array;
};

args_decl: args_decl COMMA pidentifier {
    if (!$1) $1 = json_object_new_array();
    json_object_array_add($1, $3);
    $$ = $1;
        }
| args_decl COMMA IDENTIFIER pidentifier {
    if (!$1) $1 = json_object_new_array();
    struct json_object *arg_obj = json_object_new_object();
    json_object_object_add(arg_obj, "type", json_object_new_string($3));
    json_object_object_add(arg_obj, "identifier", $4);
    json_object_array_add($1, arg_obj);
    $$ = $1;
        }
| pidentifier {
    struct json_object *arg_array = json_object_new_array();
    json_object_array_add(arg_array, $1);
    $$ = arg_array;
        }
| IDENTIFIER pidentifier {
    struct json_object *arg_array = json_object_new_array();
    struct json_object *arg_obj = json_object_new_object();
    json_object_object_add(arg_obj, "type", json_object_new_string($1));
    json_object_object_add(arg_obj, "identifier", $2);
    json_object_array_add(arg_array, arg_obj);
    $$ = arg_array;
};


args: args COMMA pidentifier {
    if (!$1) $1 = json_object_new_array();
    json_object_array_add($1, $3);
    $$ = $1;
}
| pidentifier {
    struct json_object *arg_array = json_object_new_array();
    json_object_array_add(arg_array, $1);
    $$ = arg_array;
};

expression: value {
    $$ = $1;
}
| value PLUS value {
    struct json_object *expr_obj = json_object_new_object();
    json_object_object_add(expr_obj, "type", json_object_new_string("plus"));
    json_object_object_add(expr_obj, "left", $1);
    json_object_object_add(expr_obj, "right", $3);
    $$ = expr_obj;
}
| value MINUS value {
    struct json_object *expr_obj = json_object_new_object();
    json_object_object_add(expr_obj, "type", json_object_new_string("minus"));
    json_object_object_add(expr_obj, "left", $1);
    json_object_object_add(expr_obj, "right", $3);
    $$ = expr_obj;
}
| value MULTIPLY value {
    struct json_object *expr_obj = json_object_new_object();
    json_object_object_add(expr_obj, "type", json_object_new_string("multiply"));
    json_object_object_add(expr_obj, "left", $1);
    json_object_object_add(expr_obj, "right", $3);
    $$ = expr_obj;
}
| value DIVIDE value {
    struct json_object *expr_obj = json_object_new_object();
    json_object_object_add(expr_obj, "type", json_object_new_string("divide"));
    json_object_object_add(expr_obj, "left", $1);
    json_object_object_add(expr_obj, "right", $3);
    $$ = expr_obj;
}
| value MODULO value {
    struct json_object *expr_obj = json_object_new_object();
    json_object_object_add(expr_obj, "type", json_object_new_string("modulo"));
    json_object_object_add(expr_obj, "left", $1);
    json_object_object_add(expr_obj, "right", $3);
    $$ = expr_obj;
};

condition: value EQ value {
    struct json_object *cond_obj = json_object_new_object();
    json_object_object_add(cond_obj, "type", json_object_new_string("eq"));
    json_object_object_add(cond_obj, "left", $1);
    json_object_object_add(cond_obj, "right", $3);
    $$ = cond_obj;
}
| value NE value {
    struct json_object *cond_obj = json_object_new_object();
    json_object_object_add(cond_obj, "type", json_object_new_string("ne"));
    json_object_object_add(cond_obj, "left", $1);
    json_object_object_add(cond_obj, "right", $3);
    $$ = cond_obj;
}
| value GT value {
    struct json_object *cond_obj = json_object_new_object();
    json_object_object_add(cond_obj, "type", json_object_new_string("gt"));
    json_object_object_add(cond_obj, "left", $1);
    json_object_object_add(cond_obj, "right", $3);
    $$ = cond_obj;
}
| value LT value {
    struct json_object *cond_obj = json_object_new_object();
    json_object_object_add(cond_obj, "type", json_object_new_string("lt"));
    json_object_object_add(cond_obj, "left", $1);
    json_object_object_add(cond_obj, "right", $3);
    $$ = cond_obj;
}
| value GE value {
    struct json_object *cond_obj = json_object_new_object();
    json_object_object_add(cond_obj, "type", json_object_new_string("ge"));
    json_object_object_add(cond_obj, "left", $1);
    json_object_object_add(cond_obj, "right", $3);
    $$ = cond_obj;
}
| value LE value {
    struct json_object *cond_obj = json_object_new_object();
    json_object_object_add(cond_obj, "type", json_object_new_string("le"));
    json_object_object_add(cond_obj, "left", $1);
    json_object_object_add(cond_obj, "right", $3);
    $$ = cond_obj;
};

value: number {
    $$ = json_object_new_int($1);
}
| identifier {
    $$ = $1;
};

pidentifier: IDENTIFIER {
    $$ = json_object_new_string($1);
};

number: NUMBER {
    $$ = $1;
};

identifier: pidentifier {
    $$ = $1;
}
| pidentifier LBRACKET pidentifier RBRACKET {
    struct json_object *arr_access = json_object_new_object();
    json_object_object_add(arr_access, "identifier", $1);
    json_object_object_add(arr_access, "index", $3);
    $$ = arr_access;
}
| pidentifier LBRACKET NUMBER RBRACKET {
    struct json_object *arr_access = json_object_new_object();
    json_object_object_add(arr_access, "identifier", $1);
    json_object_object_add(arr_access, "index", json_object_new_int($3));
    $$ = arr_access;
};


%%

/* Error Handling */
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s at line %d\n", s, yylineno);
}

int main(int argc, char *argv[]) {
    // Check if an output file path is provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <output_json_path>\n", argv[0]);
        return 1;
    }

    int parse_result = yyparse();
    
    // If parsing is successful, write the JSON to the specified file
    if (parse_result == 0 && program_json) {
        FILE *output_file = fopen(argv[1], "w");
        if (output_file) {
            fprintf(output_file, "%s\n", json_object_to_json_string_ext(program_json, JSON_C_TO_STRING_PRETTY));
            fclose(output_file);
            printf("Parsing successful. JSON written to %s\n", argv[1]);
        } else {
            fprintf(stderr, "Error: Could not open output file %s\n", argv[1]);
            return 1;
        }
    }
    
    return parse_result;
}
