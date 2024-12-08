/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

extern int yylineno;
extern int yylex();
void yyerror(const char *s);

// Global JSON object to store the entire program structure
struct json_object *program_json;

#line 85 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_PROGRAM = 3,                    /* PROGRAM  */
  YYSYMBOL_IS = 4,                         /* IS  */
  YYSYMBOL_BEGIN_BLOCK = 5,                /* BEGIN_BLOCK  */
  YYSYMBOL_END = 6,                        /* END  */
  YYSYMBOL_IF = 7,                         /* IF  */
  YYSYMBOL_THEN = 8,                       /* THEN  */
  YYSYMBOL_ELSE = 9,                       /* ELSE  */
  YYSYMBOL_ENDIF = 10,                     /* ENDIF  */
  YYSYMBOL_WHILE = 11,                     /* WHILE  */
  YYSYMBOL_DO = 12,                        /* DO  */
  YYSYMBOL_ENDWHILE = 13,                  /* ENDWHILE  */
  YYSYMBOL_REPEAT = 14,                    /* REPEAT  */
  YYSYMBOL_UNTIL = 15,                     /* UNTIL  */
  YYSYMBOL_FOR = 16,                       /* FOR  */
  YYSYMBOL_FROM = 17,                      /* FROM  */
  YYSYMBOL_TO = 18,                        /* TO  */
  YYSYMBOL_DOWNTO = 19,                    /* DOWNTO  */
  YYSYMBOL_ENDFOR = 20,                    /* ENDFOR  */
  YYSYMBOL_PROCEDURE = 21,                 /* PROCEDURE  */
  YYSYMBOL_READ = 22,                      /* READ  */
  YYSYMBOL_WRITE = 23,                     /* WRITE  */
  YYSYMBOL_ASSIGN = 24,                    /* ASSIGN  */
  YYSYMBOL_PLUS = 25,                      /* PLUS  */
  YYSYMBOL_MINUS = 26,                     /* MINUS  */
  YYSYMBOL_MULTIPLY = 27,                  /* MULTIPLY  */
  YYSYMBOL_DIVIDE = 28,                    /* DIVIDE  */
  YYSYMBOL_MODULO = 29,                    /* MODULO  */
  YYSYMBOL_EQ = 30,                        /* EQ  */
  YYSYMBOL_NE = 31,                        /* NE  */
  YYSYMBOL_LT = 32,                        /* LT  */
  YYSYMBOL_GT = 33,                        /* GT  */
  YYSYMBOL_LE = 34,                        /* LE  */
  YYSYMBOL_GE = 35,                        /* GE  */
  YYSYMBOL_SEMICOLON = 36,                 /* SEMICOLON  */
  YYSYMBOL_COLON = 37,                     /* COLON  */
  YYSYMBOL_COMMA = 38,                     /* COMMA  */
  YYSYMBOL_LPAREN = 39,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 40,                    /* RPAREN  */
  YYSYMBOL_LBRACKET = 41,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 42,                  /* RBRACKET  */
  YYSYMBOL_ERROR = 43,                     /* ERROR  */
  YYSYMBOL_IDENTIFIER = 44,                /* IDENTIFIER  */
  YYSYMBOL_ARRAY = 45,                     /* ARRAY  */
  YYSYMBOL_NUMBER = 46,                    /* NUMBER  */
  YYSYMBOL_YYACCEPT = 47,                  /* $accept  */
  YYSYMBOL_program_all = 48,               /* program_all  */
  YYSYMBOL_procedures = 49,                /* procedures  */
  YYSYMBOL_main = 50,                      /* main  */
  YYSYMBOL_commands = 51,                  /* commands  */
  YYSYMBOL_command = 52,                   /* command  */
  YYSYMBOL_proc_head = 53,                 /* proc_head  */
  YYSYMBOL_proc_call = 54,                 /* proc_call  */
  YYSYMBOL_declarations = 55,              /* declarations  */
  YYSYMBOL_args_decl = 56,                 /* args_decl  */
  YYSYMBOL_args = 57,                      /* args  */
  YYSYMBOL_expression = 58,                /* expression  */
  YYSYMBOL_condition = 59,                 /* condition  */
  YYSYMBOL_value = 60,                     /* value  */
  YYSYMBOL_pidentifier = 61,               /* pidentifier  */
  YYSYMBOL_number = 62,                    /* number  */
  YYSYMBOL_identifier = 63                 /* identifier  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   344

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  17
/* YYNRULES -- Number of rules.  */
#define YYNRULES  50
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  135

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   301


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    61,    61,    68,    77,    85,    87,    93,    99,   104,
     110,   117,   125,   132,   139,   146,   155,   164,   165,   171,
     178,   185,   193,   198,   207,   212,   222,   227,   235,   240,
     250,   255,   261,   264,   271,   278,   285,   292,   300,   307,
     314,   321,   328,   335,   343,   346,   350,   354,   358,   361,
     367
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "PROGRAM", "IS",
  "BEGIN_BLOCK", "END", "IF", "THEN", "ELSE", "ENDIF", "WHILE", "DO",
  "ENDWHILE", "REPEAT", "UNTIL", "FOR", "FROM", "TO", "DOWNTO", "ENDFOR",
  "PROCEDURE", "READ", "WRITE", "ASSIGN", "PLUS", "MINUS", "MULTIPLY",
  "DIVIDE", "MODULO", "EQ", "NE", "LT", "GT", "LE", "GE", "SEMICOLON",
  "COLON", "COMMA", "LPAREN", "RPAREN", "LBRACKET", "RBRACKET", "ERROR",
  "IDENTIFIER", "ARRAY", "NUMBER", "$accept", "program_all", "procedures",
  "main", "commands", "command", "proc_head", "proc_call", "declarations",
  "args_decl", "args", "expression", "condition", "value", "pidentifier",
  "number", "identifier", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-37)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -37,     5,     3,   -37,    12,   -36,   -37,    -3,   -37,    21,
     -26,   300,     2,    -6,    -1,   -23,   -27,   -27,   300,   -36,
     -36,   -27,     4,   -37,    -8,    20,    31,   300,   -36,    -2,
     300,     9,   -36,    22,   -37,   -37,    48,    87,    24,   -37,
     -37,    56,   229,    47,    51,    53,   -37,   -37,   -37,   -36,
      25,   -27,    23,    42,    57,   175,   300,   -37,   -13,   -37,
     300,   -27,   -27,   -27,   -27,   -27,   -27,   300,   -27,   -27,
     -37,   -37,    52,   -37,    54,    66,    59,    49,   -37,    61,
      63,   -37,   194,   -36,   -37,   219,   -37,   -37,   -37,   -37,
     -37,   -37,   242,    88,    32,   -36,   -37,   -37,   -37,   -37,
     -27,   -27,   -27,   -27,   -27,    98,    83,   -37,   -37,   300,
     -37,   -37,   -37,   -27,   -27,   -37,   -37,   -37,   -37,   -37,
     -37,    90,   -37,   260,   130,   131,   102,   -37,   300,   300,
     -37,   273,   283,   -37,   -37
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       5,     0,     0,     1,     0,     0,     2,     0,    46,     0,
       0,     0,     0,    24,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     9,     0,    48,     0,     0,     0,     0,
       0,     0,     0,     0,    28,    47,     0,     0,    48,    44,
      45,     0,     0,     0,     0,     0,     7,     8,    17,     0,
       0,     0,     0,    22,     0,     0,     0,    29,     0,    20,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      18,    19,     0,    31,     0,     0,     0,    32,     6,     0,
       0,     4,     0,     0,    26,     0,    38,    39,    41,    40,
      43,    42,     0,     0,     0,     0,    21,    50,    49,    10,
       0,     0,     0,     0,     0,     0,     0,     3,    27,     0,
      12,    13,    14,     0,     0,    30,    33,    34,    35,    36,
      37,     0,    25,     0,     0,     0,     0,    11,     0,     0,
      23,     0,     0,    15,    16
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -37,   -37,   -37,   -37,   -18,   -19,   -37,   -37,   132,   -37,
     -37,   -37,   -16,   111,    65,   -37,    37
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,     6,    22,    23,     9,    24,    12,    33,
      72,    76,    36,    37,    25,    39,    26
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      42,    41,    11,    47,    30,     3,     4,    27,     8,    52,
      46,    16,    55,    15,    56,    17,     7,     8,    18,    35,
      19,     8,    32,    47,     5,    14,    20,    21,    48,    78,
      16,     8,    83,    47,    17,    29,    47,    18,    82,    19,
      28,     8,    85,     8,    54,    20,    21,    28,     8,    92,
     113,   114,    93,    40,    40,    51,    60,    44,    40,    49,
      58,    50,    59,    47,    69,    50,    47,     8,    67,     8,
      10,    74,    13,    47,   100,   101,   102,   103,   104,    13,
      34,    38,    38,    79,    43,    38,    38,    70,    40,    71,
      95,   123,    96,    53,    80,    99,    97,    57,    40,    40,
      40,    40,    40,    40,    47,    40,    40,   105,    98,   106,
     131,   132,    47,    47,    73,    75,    38,    61,    62,    63,
      64,    65,    66,    84,   112,   122,    38,    38,    38,    38,
      38,    38,    45,    38,    38,   121,   126,    40,    40,    40,
      40,    40,   128,   129,   130,     0,    31,     0,   108,     0,
      40,    40,     0,     0,     0,     0,     0,     0,     0,     0,
     115,     0,    77,     0,     0,    38,    38,    38,    38,    38,
       0,     0,    86,    87,    88,    89,    90,    91,    38,    38,
      94,    81,    16,     0,     0,     0,    17,     0,     0,    18,
       0,    19,     0,     0,     0,     0,     0,    20,    21,     0,
     107,    16,     0,     0,     0,    17,     0,     0,    18,     0,
      19,   116,   117,   118,   119,   120,    20,    21,     0,     8,
       0,     0,     0,     0,   124,   125,    16,     0,   109,   110,
      17,     0,     0,    18,     0,    19,    16,     0,     8,     0,
      17,    20,    21,    18,    68,    19,     0,     0,     0,    16,
       0,    20,    21,    17,     0,   111,    18,     0,    19,     0,
       0,     0,     0,     8,    20,    21,     0,    16,     0,     0,
     127,    17,     0,     8,    18,     0,    19,     0,     0,     0,
      16,     0,    20,    21,    17,     0,     8,    18,     0,    19,
      16,     0,     0,   133,    17,    20,    21,    18,     0,    19,
       0,     0,     0,   134,     8,    20,    21,    16,     0,     0,
       0,    17,     0,     0,    18,     0,    19,     8,     0,     0,
       0,     0,    20,    21,     0,     0,     0,     8,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     8
};

static const yytype_int16 yycheck[] =
{
      18,    17,     5,    22,     5,     0,     3,     5,    44,    27,
       6,     7,    30,    39,     5,    11,     4,    44,    14,    46,
      16,    44,    45,    42,    21,     4,    22,    23,    36,     6,
       7,    44,    45,    52,    11,    41,    55,    14,    56,    16,
      38,    44,    60,    44,    46,    22,    23,    38,    44,    67,
      18,    19,    68,    16,    17,    24,     8,    20,    21,    39,
      38,    41,    40,    82,    17,    41,    85,    44,    12,    44,
       5,    46,     7,    92,    25,    26,    27,    28,    29,    14,
      15,    16,    17,    41,    19,    20,    21,    36,    51,    36,
      38,   109,    40,    28,    37,    36,    42,    32,    61,    62,
      63,    64,    65,    66,   123,    68,    69,    46,    42,    46,
     128,   129,   131,   132,    49,    50,    51,    30,    31,    32,
      33,    34,    35,    58,    36,    42,    61,    62,    63,    64,
      65,    66,    21,    68,    69,    37,    46,   100,   101,   102,
     103,   104,    12,    12,    42,    -1,    14,    -1,    83,    -1,
     113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    51,    -1,    -1,   100,   101,   102,   103,   104,
      -1,    -1,    61,    62,    63,    64,    65,    66,   113,   114,
      69,     6,     7,    -1,    -1,    -1,    11,    -1,    -1,    14,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    22,    23,    -1,
       6,     7,    -1,    -1,    -1,    11,    -1,    -1,    14,    -1,
      16,   100,   101,   102,   103,   104,    22,    23,    -1,    44,
      -1,    -1,    -1,    -1,   113,   114,     7,    -1,     9,    10,
      11,    -1,    -1,    14,    -1,    16,     7,    -1,    44,    -1,
      11,    22,    23,    14,    15,    16,    -1,    -1,    -1,     7,
      -1,    22,    23,    11,    -1,    13,    14,    -1,    16,    -1,
      -1,    -1,    -1,    44,    22,    23,    -1,     7,    -1,    -1,
      10,    11,    -1,    44,    14,    -1,    16,    -1,    -1,    -1,
       7,    -1,    22,    23,    11,    -1,    44,    14,    -1,    16,
       7,    -1,    -1,    20,    11,    22,    23,    14,    -1,    16,
      -1,    -1,    -1,    20,    44,    22,    23,     7,    -1,    -1,
      -1,    11,    -1,    -1,    14,    -1,    16,    44,    -1,    -1,
      -1,    -1,    22,    23,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    48,    49,     0,     3,    21,    50,     4,    44,    53,
      61,     5,    55,    61,     4,    39,     7,    11,    14,    16,
      22,    23,    51,    52,    54,    61,    63,     5,    38,    41,
       5,    55,    45,    56,    61,    46,    59,    60,    61,    62,
      63,    59,    51,    61,    63,    60,     6,    52,    36,    39,
      41,    24,    51,    61,    46,    51,     5,    61,    38,    40,
       8,    30,    31,    32,    33,    34,    35,    12,    15,    17,
      36,    36,    57,    61,    46,    61,    58,    60,     6,    41,
      37,     6,    51,    45,    61,    51,    60,    60,    60,    60,
      60,    60,    51,    59,    60,    38,    40,    42,    42,    36,
      25,    26,    27,    28,    29,    46,    46,     6,    61,     9,
      10,    13,    36,    18,    19,    61,    60,    60,    60,    60,
      60,    37,    42,    51,    60,    60,    46,    10,    12,    12,
      42,    51,    51,    20,    20
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    47,    48,    49,    49,    49,    50,    50,    51,    51,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      53,    54,    55,    55,    55,    55,    56,    56,    56,    56,
      57,    57,    58,    58,    58,    58,    58,    58,    59,    59,
      59,    59,    59,    59,    60,    60,    61,    62,    63,    63,
      63
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     8,     7,     0,     6,     5,     2,     1,
       4,     7,     5,     5,     5,     9,     9,     2,     3,     3,
       4,     4,     3,     8,     1,     6,     3,     4,     1,     2,
       3,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     4,
       4
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program_all: procedures main  */
#line 61 "parser.y"
                             {
    program_json = json_object_new_object();
    json_object_object_add(program_json, "procedures", (yyvsp[-1].json_obj) ? (yyvsp[-1].json_obj) : json_object_new_null());
    json_object_object_add(program_json, "main", (yyvsp[0].json_obj));
    (yyval.json_obj) = program_json;
}
#line 1263 "parser.tab.c"
    break;

  case 3: /* procedures: procedures PROCEDURE proc_head IS declarations BEGIN_BLOCK commands END  */
#line 68 "parser.y"
                                                                                    {
    if (!(yyvsp[-7].json_obj)) (yyvsp[-7].json_obj) = json_object_new_array();
    struct json_object *proc_obj = json_object_new_object();
    json_object_object_add(proc_obj, "head", (yyvsp[-5].json_obj));
    json_object_object_add(proc_obj, "declarations", (yyvsp[-3].json_obj));
    json_object_object_add(proc_obj, "commands", (yyvsp[-1].json_obj));
    json_object_array_add((yyvsp[-7].json_obj), proc_obj);
    (yyval.json_obj) = (yyvsp[-7].json_obj);
}
#line 1277 "parser.tab.c"
    break;

  case 4: /* procedures: procedures PROCEDURE proc_head IS BEGIN_BLOCK commands END  */
#line 77 "parser.y"
                                                             {
    if (!(yyvsp[-6].json_obj)) (yyvsp[-6].json_obj) = json_object_new_array();
    struct json_object *proc_obj = json_object_new_object();
    json_object_object_add(proc_obj, "head", (yyvsp[-4].json_obj));
    json_object_object_add(proc_obj, "commands", (yyvsp[-1].json_obj));
    json_object_array_add((yyvsp[-6].json_obj), proc_obj);
    (yyval.json_obj) = (yyvsp[-6].json_obj);
}
#line 1290 "parser.tab.c"
    break;

  case 5: /* procedures: %empty  */
#line 85 "parser.y"
              { (yyval.json_obj) = NULL; }
#line 1296 "parser.tab.c"
    break;

  case 6: /* main: PROGRAM IS declarations BEGIN_BLOCK commands END  */
#line 87 "parser.y"
                                                       {
    struct json_object *main_obj = json_object_new_object();
    json_object_object_add(main_obj, "declarations", (yyvsp[-3].json_obj));
    json_object_object_add(main_obj, "commands", (yyvsp[-1].json_obj));
    (yyval.json_obj) = main_obj;
}
#line 1307 "parser.tab.c"
    break;

  case 7: /* main: PROGRAM IS BEGIN_BLOCK commands END  */
#line 93 "parser.y"
                                      {
    struct json_object *main_obj = json_object_new_object();
    json_object_object_add(main_obj, "commands", (yyvsp[-1].json_obj));
    (yyval.json_obj) = main_obj;
}
#line 1317 "parser.tab.c"
    break;

  case 8: /* commands: commands command  */
#line 99 "parser.y"
                           {
    if (!(yyvsp[-1].json_obj)) (yyvsp[-1].json_obj) = json_object_new_array();
    json_object_array_add((yyvsp[-1].json_obj), (yyvsp[0].json_obj));
    (yyval.json_obj) = (yyvsp[-1].json_obj);
}
#line 1327 "parser.tab.c"
    break;

  case 9: /* commands: command  */
#line 104 "parser.y"
          {
    struct json_object *cmd_array = json_object_new_array();
    json_object_array_add(cmd_array, (yyvsp[0].json_obj));
    (yyval.json_obj) = cmd_array;
}
#line 1337 "parser.tab.c"
    break;

  case 10: /* command: identifier ASSIGN expression SEMICOLON  */
#line 110 "parser.y"
                                                {
    struct json_object *cmd_obj = json_object_new_object();
    json_object_object_add(cmd_obj, "type", json_object_new_string("assign"));
    json_object_object_add(cmd_obj, "identifier", (yyvsp[-3].json_obj));
    json_object_object_add(cmd_obj, "expression", (yyvsp[-1].json_obj));
    (yyval.json_obj) = cmd_obj;
}
#line 1349 "parser.tab.c"
    break;

  case 11: /* command: IF condition THEN commands ELSE commands ENDIF  */
#line 117 "parser.y"
                                                 {
    struct json_object *cmd_obj = json_object_new_object();
    json_object_object_add(cmd_obj, "type", json_object_new_string("if_else"));
    json_object_object_add(cmd_obj, "condition", (yyvsp[-5].json_obj));
    json_object_object_add(cmd_obj, "then_commands", (yyvsp[-3].json_obj));
    json_object_object_add(cmd_obj, "else_commands", (yyvsp[-1].json_obj));
    (yyval.json_obj) = cmd_obj;
}
#line 1362 "parser.tab.c"
    break;

  case 12: /* command: IF condition THEN commands ENDIF  */
#line 125 "parser.y"
                                   {
    struct json_object *cmd_obj = json_object_new_object();
    json_object_object_add(cmd_obj, "type", json_object_new_string("if"));
    json_object_object_add(cmd_obj, "condition", (yyvsp[-3].json_obj));
    json_object_object_add(cmd_obj, "commands", (yyvsp[-1].json_obj));
    (yyval.json_obj) = cmd_obj;
}
#line 1374 "parser.tab.c"
    break;

  case 13: /* command: WHILE condition DO commands ENDWHILE  */
#line 132 "parser.y"
                                       {
    struct json_object *cmd_obj = json_object_new_object();
    json_object_object_add(cmd_obj, "type", json_object_new_string("while"));
    json_object_object_add(cmd_obj, "condition", (yyvsp[-3].json_obj));
    json_object_object_add(cmd_obj, "commands", (yyvsp[-1].json_obj));
    (yyval.json_obj) = cmd_obj;
}
#line 1386 "parser.tab.c"
    break;

  case 14: /* command: REPEAT commands UNTIL condition SEMICOLON  */
#line 139 "parser.y"
                                            {
    struct json_object *cmd_obj = json_object_new_object();
    json_object_object_add(cmd_obj, "type", json_object_new_string("repeat"));
    json_object_object_add(cmd_obj, "commands", (yyvsp[-3].json_obj));
    json_object_object_add(cmd_obj, "condition", (yyvsp[-1].json_obj));
    (yyval.json_obj) = cmd_obj;
}
#line 1398 "parser.tab.c"
    break;

  case 15: /* command: FOR pidentifier FROM value TO value DO commands ENDFOR  */
#line 146 "parser.y"
                                                         {
    struct json_object *cmd_obj = json_object_new_object();
    json_object_object_add(cmd_obj, "type", json_object_new_string("for_to"));
    json_object_object_add(cmd_obj, "identifier", (yyvsp[-7].json_obj));
    json_object_object_add(cmd_obj, "from", (yyvsp[-5].json_obj));
    json_object_object_add(cmd_obj, "to", (yyvsp[-3].json_obj));
    json_object_object_add(cmd_obj, "commands", (yyvsp[-1].json_obj));
    (yyval.json_obj) = cmd_obj;
}
#line 1412 "parser.tab.c"
    break;

  case 16: /* command: FOR pidentifier FROM value DOWNTO value DO commands ENDFOR  */
#line 155 "parser.y"
                                                             {
    struct json_object *cmd_obj = json_object_new_object();
    json_object_object_add(cmd_obj, "type", json_object_new_string("for_downto"));
    json_object_object_add(cmd_obj, "identifier", (yyvsp[-7].json_obj));
    json_object_object_add(cmd_obj, "from", (yyvsp[-5].json_obj));
    json_object_object_add(cmd_obj, "downto", (yyvsp[-3].json_obj));
    json_object_object_add(cmd_obj, "commands", (yyvsp[-1].json_obj));
    (yyval.json_obj) = cmd_obj;
}
#line 1426 "parser.tab.c"
    break;

  case 17: /* command: proc_call SEMICOLON  */
#line 164 "parser.y"
                      { (yyval.json_obj) = (yyvsp[-1].json_obj); }
#line 1432 "parser.tab.c"
    break;

  case 18: /* command: READ identifier SEMICOLON  */
#line 165 "parser.y"
                            {
    struct json_object *cmd_obj = json_object_new_object();
    json_object_object_add(cmd_obj, "type", json_object_new_string("read"));
    json_object_object_add(cmd_obj, "identifier", (yyvsp[-1].json_obj));
    (yyval.json_obj) = cmd_obj;
}
#line 1443 "parser.tab.c"
    break;

  case 19: /* command: WRITE value SEMICOLON  */
#line 171 "parser.y"
                        {
    struct json_object *cmd_obj = json_object_new_object();
    json_object_object_add(cmd_obj, "type", json_object_new_string("write"));
    json_object_object_add(cmd_obj, "value", (yyvsp[-1].json_obj));
    (yyval.json_obj) = cmd_obj;
}
#line 1454 "parser.tab.c"
    break;

  case 20: /* proc_head: pidentifier LPAREN args_decl RPAREN  */
#line 178 "parser.y"
                                               {
    struct json_object *head_obj = json_object_new_object();
    json_object_object_add(head_obj, "identifier", (yyvsp[-3].json_obj));
    json_object_object_add(head_obj, "arguments", (yyvsp[-1].json_obj) ? (yyvsp[-1].json_obj) : json_object_new_null());
    (yyval.json_obj) = head_obj;
}
#line 1465 "parser.tab.c"
    break;

  case 21: /* proc_call: pidentifier LPAREN args RPAREN  */
#line 185 "parser.y"
                                          {
    struct json_object *call_obj = json_object_new_object();
    json_object_object_add(call_obj, "type", json_object_new_string("procedure_call"));
    json_object_object_add(call_obj, "identifier", (yyvsp[-3].json_obj));
    json_object_object_add(call_obj, "arguments", (yyvsp[-1].json_obj) ? (yyvsp[-1].json_obj) : json_object_new_null());
    (yyval.json_obj) = call_obj;
}
#line 1477 "parser.tab.c"
    break;

  case 22: /* declarations: declarations COMMA pidentifier  */
#line 193 "parser.y"
                                             {
    if (!(yyvsp[-2].json_obj)) (yyvsp[-2].json_obj) = json_object_new_array();
    json_object_array_add((yyvsp[-2].json_obj), (yyvsp[0].json_obj));
    (yyval.json_obj) = (yyvsp[-2].json_obj);
}
#line 1487 "parser.tab.c"
    break;

  case 23: /* declarations: declarations COMMA pidentifier LBRACKET NUMBER COLON NUMBER RBRACKET  */
#line 198 "parser.y"
                                                                       {
    if (!(yyvsp[-7].json_obj)) (yyvsp[-7].json_obj) = json_object_new_array();
    struct json_object *arr_decl = json_object_new_object();
    json_object_object_add(arr_decl, "identifier", (yyvsp[-5].json_obj));
    json_object_object_add(arr_decl, "start_index", json_object_new_int((yyvsp[-3].int_val)));
    json_object_object_add(arr_decl, "end_index", json_object_new_int((yyvsp[-1].int_val)));
    json_object_array_add((yyvsp[-7].json_obj), arr_decl);
    (yyval.json_obj) = (yyvsp[-7].json_obj);
}
#line 1501 "parser.tab.c"
    break;

  case 24: /* declarations: pidentifier  */
#line 207 "parser.y"
              {
    struct json_object *decl_array = json_object_new_array();
    json_object_array_add(decl_array, (yyvsp[0].json_obj));
    (yyval.json_obj) = decl_array;
}
#line 1511 "parser.tab.c"
    break;

  case 25: /* declarations: pidentifier LBRACKET NUMBER COLON NUMBER RBRACKET  */
#line 212 "parser.y"
                                                    {
    struct json_object *decl_array = json_object_new_array();
    struct json_object *arr_decl = json_object_new_object();
    json_object_object_add(arr_decl, "identifier", (yyvsp[-5].json_obj));
    json_object_object_add(arr_decl, "start_index", json_object_new_int((yyvsp[-3].int_val)));
    json_object_object_add(arr_decl, "end_index", json_object_new_int((yyvsp[-1].int_val)));
    json_object_array_add(decl_array, arr_decl);
    (yyval.json_obj) = decl_array;
}
#line 1525 "parser.tab.c"
    break;

  case 26: /* args_decl: args_decl COMMA pidentifier  */
#line 222 "parser.y"
                                       {
    if (!(yyvsp[-2].json_obj)) (yyvsp[-2].json_obj) = json_object_new_array();
    json_object_array_add((yyvsp[-2].json_obj), (yyvsp[0].json_obj));
    (yyval.json_obj) = (yyvsp[-2].json_obj);
        }
#line 1535 "parser.tab.c"
    break;

  case 27: /* args_decl: args_decl COMMA ARRAY pidentifier  */
#line 227 "parser.y"
                                    {
    if (!(yyvsp[-3].json_obj)) (yyvsp[-3].json_obj) = json_object_new_array();
    struct json_object *arg_obj = json_object_new_object();
    json_object_object_add(arg_obj, "type", json_object_new_string("array"));
    json_object_object_add(arg_obj, "identifier", (yyvsp[0].json_obj));
    json_object_array_add((yyvsp[-3].json_obj), arg_obj);
    (yyval.json_obj) = (yyvsp[-3].json_obj);
        }
#line 1548 "parser.tab.c"
    break;

  case 28: /* args_decl: pidentifier  */
#line 235 "parser.y"
              {
    struct json_object *arg_array = json_object_new_array();
    json_object_array_add(arg_array, (yyvsp[0].json_obj));
    (yyval.json_obj) = arg_array;
        }
#line 1558 "parser.tab.c"
    break;

  case 29: /* args_decl: ARRAY pidentifier  */
#line 240 "parser.y"
                    {
    struct json_object *arg_array = json_object_new_array();
    struct json_object *arg_obj = json_object_new_object();
    json_object_object_add(arg_obj, "type", json_object_new_string("array"));
    json_object_object_add(arg_obj, "identifier", (yyvsp[0].json_obj));
    json_object_array_add(arg_array, arg_obj);
    (yyval.json_obj) = arg_array;
}
#line 1571 "parser.tab.c"
    break;

  case 30: /* args: args COMMA pidentifier  */
#line 250 "parser.y"
                             {
    if (!(yyvsp[-2].json_obj)) (yyvsp[-2].json_obj) = json_object_new_array();
    json_object_array_add((yyvsp[-2].json_obj), (yyvsp[0].json_obj));
    (yyval.json_obj) = (yyvsp[-2].json_obj);
}
#line 1581 "parser.tab.c"
    break;

  case 31: /* args: pidentifier  */
#line 255 "parser.y"
              {
    struct json_object *arg_array = json_object_new_array();
    json_object_array_add(arg_array, (yyvsp[0].json_obj));
    (yyval.json_obj) = arg_array;
}
#line 1591 "parser.tab.c"
    break;

  case 32: /* expression: value  */
#line 261 "parser.y"
                  {
    (yyval.json_obj) = (yyvsp[0].json_obj);
}
#line 1599 "parser.tab.c"
    break;

  case 33: /* expression: value PLUS value  */
#line 264 "parser.y"
                   {
    struct json_object *expr_obj = json_object_new_object();
    json_object_object_add(expr_obj, "type", json_object_new_string("plus"));
    json_object_object_add(expr_obj, "left", (yyvsp[-2].json_obj));
    json_object_object_add(expr_obj, "right", (yyvsp[0].json_obj));
    (yyval.json_obj) = expr_obj;
}
#line 1611 "parser.tab.c"
    break;

  case 34: /* expression: value MINUS value  */
#line 271 "parser.y"
                    {
    struct json_object *expr_obj = json_object_new_object();
    json_object_object_add(expr_obj, "type", json_object_new_string("minus"));
    json_object_object_add(expr_obj, "left", (yyvsp[-2].json_obj));
    json_object_object_add(expr_obj, "right", (yyvsp[0].json_obj));
    (yyval.json_obj) = expr_obj;
}
#line 1623 "parser.tab.c"
    break;

  case 35: /* expression: value MULTIPLY value  */
#line 278 "parser.y"
                       {
    struct json_object *expr_obj = json_object_new_object();
    json_object_object_add(expr_obj, "type", json_object_new_string("multiply"));
    json_object_object_add(expr_obj, "left", (yyvsp[-2].json_obj));
    json_object_object_add(expr_obj, "right", (yyvsp[0].json_obj));
    (yyval.json_obj) = expr_obj;
}
#line 1635 "parser.tab.c"
    break;

  case 36: /* expression: value DIVIDE value  */
#line 285 "parser.y"
                     {
    struct json_object *expr_obj = json_object_new_object();
    json_object_object_add(expr_obj, "type", json_object_new_string("divide"));
    json_object_object_add(expr_obj, "left", (yyvsp[-2].json_obj));
    json_object_object_add(expr_obj, "right", (yyvsp[0].json_obj));
    (yyval.json_obj) = expr_obj;
}
#line 1647 "parser.tab.c"
    break;

  case 37: /* expression: value MODULO value  */
#line 292 "parser.y"
                     {
    struct json_object *expr_obj = json_object_new_object();
    json_object_object_add(expr_obj, "type", json_object_new_string("modulo"));
    json_object_object_add(expr_obj, "left", (yyvsp[-2].json_obj));
    json_object_object_add(expr_obj, "right", (yyvsp[0].json_obj));
    (yyval.json_obj) = expr_obj;
}
#line 1659 "parser.tab.c"
    break;

  case 38: /* condition: value EQ value  */
#line 300 "parser.y"
                          {
    struct json_object *cond_obj = json_object_new_object();
    json_object_object_add(cond_obj, "type", json_object_new_string("eq"));
    json_object_object_add(cond_obj, "left", (yyvsp[-2].json_obj));
    json_object_object_add(cond_obj, "right", (yyvsp[0].json_obj));
    (yyval.json_obj) = cond_obj;
}
#line 1671 "parser.tab.c"
    break;

  case 39: /* condition: value NE value  */
#line 307 "parser.y"
                 {
    struct json_object *cond_obj = json_object_new_object();
    json_object_object_add(cond_obj, "type", json_object_new_string("ne"));
    json_object_object_add(cond_obj, "left", (yyvsp[-2].json_obj));
    json_object_object_add(cond_obj, "right", (yyvsp[0].json_obj));
    (yyval.json_obj) = cond_obj;
}
#line 1683 "parser.tab.c"
    break;

  case 40: /* condition: value GT value  */
#line 314 "parser.y"
                 {
    struct json_object *cond_obj = json_object_new_object();
    json_object_object_add(cond_obj, "type", json_object_new_string("gt"));
    json_object_object_add(cond_obj, "left", (yyvsp[-2].json_obj));
    json_object_object_add(cond_obj, "right", (yyvsp[0].json_obj));
    (yyval.json_obj) = cond_obj;
}
#line 1695 "parser.tab.c"
    break;

  case 41: /* condition: value LT value  */
#line 321 "parser.y"
                 {
    struct json_object *cond_obj = json_object_new_object();
    json_object_object_add(cond_obj, "type", json_object_new_string("lt"));
    json_object_object_add(cond_obj, "left", (yyvsp[-2].json_obj));
    json_object_object_add(cond_obj, "right", (yyvsp[0].json_obj));
    (yyval.json_obj) = cond_obj;
}
#line 1707 "parser.tab.c"
    break;

  case 42: /* condition: value GE value  */
#line 328 "parser.y"
                 {
    struct json_object *cond_obj = json_object_new_object();
    json_object_object_add(cond_obj, "type", json_object_new_string("ge"));
    json_object_object_add(cond_obj, "left", (yyvsp[-2].json_obj));
    json_object_object_add(cond_obj, "right", (yyvsp[0].json_obj));
    (yyval.json_obj) = cond_obj;
}
#line 1719 "parser.tab.c"
    break;

  case 43: /* condition: value LE value  */
#line 335 "parser.y"
                 {
    struct json_object *cond_obj = json_object_new_object();
    json_object_object_add(cond_obj, "type", json_object_new_string("le"));
    json_object_object_add(cond_obj, "left", (yyvsp[-2].json_obj));
    json_object_object_add(cond_obj, "right", (yyvsp[0].json_obj));
    (yyval.json_obj) = cond_obj;
}
#line 1731 "parser.tab.c"
    break;

  case 44: /* value: number  */
#line 343 "parser.y"
              {
    (yyval.json_obj) = json_object_new_int((yyvsp[0].json_obj));
}
#line 1739 "parser.tab.c"
    break;

  case 45: /* value: identifier  */
#line 346 "parser.y"
             {
    (yyval.json_obj) = (yyvsp[0].json_obj);
}
#line 1747 "parser.tab.c"
    break;

  case 46: /* pidentifier: IDENTIFIER  */
#line 350 "parser.y"
                        {
    (yyval.json_obj) = json_object_new_string((yyvsp[0].str_val));
}
#line 1755 "parser.tab.c"
    break;

  case 47: /* number: NUMBER  */
#line 354 "parser.y"
               {
    (yyval.json_obj) = (yyvsp[0].int_val);
}
#line 1763 "parser.tab.c"
    break;

  case 48: /* identifier: pidentifier  */
#line 358 "parser.y"
                        {
    (yyval.json_obj) = (yyvsp[0].json_obj);
}
#line 1771 "parser.tab.c"
    break;

  case 49: /* identifier: pidentifier LBRACKET pidentifier RBRACKET  */
#line 361 "parser.y"
                                            {
    struct json_object *arr_access = json_object_new_object();
    json_object_object_add(arr_access, "identifier", (yyvsp[-3].json_obj));
    json_object_object_add(arr_access, "index", (yyvsp[-1].json_obj));
    (yyval.json_obj) = arr_access;
}
#line 1782 "parser.tab.c"
    break;

  case 50: /* identifier: pidentifier LBRACKET NUMBER RBRACKET  */
#line 367 "parser.y"
                                       {
    struct json_object *arr_access = json_object_new_object();
    json_object_object_add(arr_access, "identifier", (yyvsp[-3].json_obj));
    json_object_object_add(arr_access, "index", json_object_new_int((yyvsp[-1].int_val)));
    (yyval.json_obj) = arr_access;
}
#line 1793 "parser.tab.c"
    break;


#line 1797 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 374 "parser.y"


/* Error Handling */
void yyerror(const char *s) {
    exit(1); // Stop parsing on error
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
