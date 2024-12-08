/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    PROGRAM = 258,                 /* PROGRAM  */
    IS = 259,                      /* IS  */
    BEGIN_BLOCK = 260,             /* BEGIN_BLOCK  */
    END = 261,                     /* END  */
    IF = 262,                      /* IF  */
    THEN = 263,                    /* THEN  */
    ELSE = 264,                    /* ELSE  */
    ENDIF = 265,                   /* ENDIF  */
    WHILE = 266,                   /* WHILE  */
    DO = 267,                      /* DO  */
    ENDWHILE = 268,                /* ENDWHILE  */
    REPEAT = 269,                  /* REPEAT  */
    UNTIL = 270,                   /* UNTIL  */
    FOR = 271,                     /* FOR  */
    FROM = 272,                    /* FROM  */
    TO = 273,                      /* TO  */
    DOWNTO = 274,                  /* DOWNTO  */
    ENDFOR = 275,                  /* ENDFOR  */
    PROCEDURE = 276,               /* PROCEDURE  */
    READ = 277,                    /* READ  */
    WRITE = 278,                   /* WRITE  */
    ASSIGN = 279,                  /* ASSIGN  */
    PLUS = 280,                    /* PLUS  */
    MINUS = 281,                   /* MINUS  */
    MULTIPLY = 282,                /* MULTIPLY  */
    DIVIDE = 283,                  /* DIVIDE  */
    MODULO = 284,                  /* MODULO  */
    EQ = 285,                      /* EQ  */
    NE = 286,                      /* NE  */
    LT = 287,                      /* LT  */
    GT = 288,                      /* GT  */
    LE = 289,                      /* LE  */
    GE = 290,                      /* GE  */
    SEMICOLON = 291,               /* SEMICOLON  */
    COLON = 292,                   /* COLON  */
    COMMA = 293,                   /* COMMA  */
    LPAREN = 294,                  /* LPAREN  */
    RPAREN = 295,                  /* RPAREN  */
    LBRACKET = 296,                /* LBRACKET  */
    RBRACKET = 297,                /* RBRACKET  */
    ERROR = 298,                   /* ERROR  */
    IDENTIFIER = 299,              /* IDENTIFIER  */
    NUMBER = 300                   /* NUMBER  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 45 "parser.y"

    int int_val;
    char *str_val;
    struct json_object *json_obj;

#line 115 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
