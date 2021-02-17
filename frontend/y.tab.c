/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.3.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "tiger.y" /* yacc.c:337  */

#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "symbol.h" 
#include "error.h"
#include "absyn.h"

int yylex(void); /* function prototype */

A_exp absyn_root;

void yyerror(char *s)
{
 error(tokPos, "%s", s);
 exit(1);
}

#line 89 "y.tab.c" /* yacc.c:337  */
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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    STRING = 259,
    INT = 260,
    FLOAT = 261,
    COMMA = 262,
    COLON = 263,
    SEMICOLON = 264,
    LPAREN = 265,
    RPAREN = 266,
    LBRACK = 267,
    RBRACK = 268,
    LBRACE = 269,
    RBRACE = 270,
    DOT = 271,
    PLUS = 272,
    MINUS = 273,
    TIMES = 274,
    DIVIDE = 275,
    EQ = 276,
    NEQ = 277,
    LT = 278,
    LE = 279,
    GT = 280,
    GE = 281,
    AND = 282,
    OR = 283,
    ASSIGN = 284,
    ARRAY = 285,
    IF = 286,
    THEN = 287,
    ELSE = 288,
    WHILE = 289,
    FOR = 290,
    TO = 291,
    DO = 292,
    LET = 293,
    IN = 294,
    END = 295,
    OF = 296,
    BREAK = 297,
    NIL = 298,
    FUNCTION = 299,
    VAR = 300,
    TYPE = 301,
    UMINUS = 302
  };
#endif
/* Tokens.  */
#define ID 258
#define STRING 259
#define INT 260
#define FLOAT 261
#define COMMA 262
#define COLON 263
#define SEMICOLON 264
#define LPAREN 265
#define RPAREN 266
#define LBRACK 267
#define RBRACK 268
#define LBRACE 269
#define RBRACE 270
#define DOT 271
#define PLUS 272
#define MINUS 273
#define TIMES 274
#define DIVIDE 275
#define EQ 276
#define NEQ 277
#define LT 278
#define LE 279
#define GT 280
#define GE 281
#define AND 282
#define OR 283
#define ASSIGN 284
#define ARRAY 285
#define IF 286
#define THEN 287
#define ELSE 288
#define WHILE 289
#define FOR 290
#define TO 291
#define DO 292
#define LET 293
#define IN 294
#define END 295
#define OF 296
#define BREAK 297
#define NIL 298
#define FUNCTION 299
#define VAR 300
#define TYPE 301
#define UMINUS 302

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 21 "tiger.y" /* yacc.c:352  */

	int pos;
	int ival;
    float fval;
	string sval;
	A_var var;
	A_exp exp;
	A_dec dec;
	A_decList decs;
	A_expList exps;
	A_ty ty;
	S_symbol symbol;
	/* et cetera */
	A_fieldList fields;
    A_efieldList efields;
    A_fundec fundec;
    A_fundecList fundecs;
    A_namety namety;
	A_nametyList nametys;
	

#line 248 "y.tab.c" /* yacc.c:352  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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


#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  35
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   350

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  17
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  139

#define YYUNDEFTOK  2
#define YYMAXUTOK   302

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
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
      45,    46,    47
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    94,    94,   100,   101,   103,   104,   105,   108,   110,
     111,   113,   114,   115,   117,   118,   119,   124,   125,   128,
     130,   133,   134,   140,   141,   149,   151,   160,   165,   168,
     170,   174,   177,   180,   181,   183,   184,   185,   188,   189,
     190,   191,   192,   195,   196,   197,   198,   199,   200,   204,
     205,   208,   210,   211,   212,   215,   218,   221,   224,   227,
     230,   234,   238,   240,   241,   242,   245
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "STRING", "INT", "FLOAT", "COMMA",
  "COLON", "SEMICOLON", "LPAREN", "RPAREN", "LBRACK", "RBRACK", "LBRACE",
  "RBRACE", "DOT", "PLUS", "MINUS", "TIMES", "DIVIDE", "EQ", "NEQ", "LT",
  "LE", "GT", "GE", "AND", "OR", "ASSIGN", "ARRAY", "IF", "THEN", "ELSE",
  "WHILE", "FOR", "TO", "DO", "LET", "IN", "END", "OF", "BREAK", "NIL",
  "FUNCTION", "VAR", "TYPE", "UMINUS", "$accept", "program", "decs", "dec",
  "tydec", "tydecs", "ty", "tyfields", "vardec", "fundec", "fundecs",
  "lvalue", "exp", "paras", "efields", "expseq", "explist", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302
};
# endif

#define YYPACT_NINF -108

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-108)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      70,    11,  -108,  -108,    70,    70,    70,    70,     5,   -15,
    -108,  -108,    18,   -12,   291,    47,    70,     8,   198,     9,
      16,  -108,   247,   156,     3,    30,    31,    32,    -3,   -15,
       7,  -108,  -108,    10,  -108,  -108,    52,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
    -108,   122,    45,   263,    41,    48,    70,  -108,  -108,    70,
      70,    70,    54,    -5,    46,    70,  -108,  -108,  -108,    56,
     291,    -6,    -6,  -108,  -108,   314,   314,   314,   314,   314,
     314,   324,   303,    70,  -108,    25,    70,  -108,  -108,   230,
     291,   210,    66,    67,    70,    -2,    36,  -108,    70,  -108,
      70,   144,    70,    70,    63,    61,    50,   291,  -108,    66,
      43,  -108,  -108,   279,   291,     8,   291,   177,    74,     1,
      70,    72,    88,  -108,  -108,    70,    85,    90,    70,   291,
    -108,  -108,   291,    66,    75,   291,  -108,    70,   291
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    23,    32,    31,    63,     0,     0,     0,     0,     0,
      28,    29,     0,    27,     2,     0,     0,    52,    64,     0,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     4,
       9,     5,     6,    21,     7,     1,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      33,    36,     0,     0,     0,     0,    63,    66,    30,     0,
       0,     0,     0,     0,     0,    63,     3,    10,    22,    24,
      58,    38,    39,    40,    41,    43,    44,    46,    48,    45,
      47,    49,    50,    35,    34,    25,     0,    51,    65,    57,
      59,     0,    14,     0,     0,     0,     0,    62,     0,    37,
       0,    53,     0,     0,     0,     0,     0,    17,    11,    14,
       0,     8,    61,     0,    55,    52,    56,     0,     0,     0,
       0,     0,     0,    26,    54,     0,    15,     0,     0,    18,
      12,    13,    60,    14,     0,    19,    16,     0,    20
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -108,  -108,    68,  -108,  -108,    65,  -108,  -107,  -108,  -108,
      73,  -108,     0,    24,   -16,    44,   -46
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    12,    28,    29,    30,    31,   111,   105,    32,    33,
      34,    13,    18,    52,    55,    19,    20
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      14,   108,   121,    93,    36,    21,    22,    23,    24,   127,
      88,    54,   109,    40,    41,    51,    53,    37,    35,    97,
      57,    15,   128,    16,    94,    17,   136,    58,   110,    25,
      26,    27,    61,    62,    63,    64,    65,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
       1,     2,     3,    27,    25,    69,    84,     4,    50,    89,
      90,    91,    86,    87,    92,     5,   100,    95,    98,   104,
     106,   118,   119,     1,     2,     3,   112,   126,     6,   120,
       4,     7,     8,    51,   122,     9,   101,   130,     5,    10,
      11,   131,   133,   134,   107,    67,   137,    66,   113,   124,
     114,     6,   116,   117,     7,     8,    68,    99,     9,    96,
       0,     0,    10,    11,     0,     0,     0,     0,     0,     0,
     129,     0,     0,     0,     0,   132,     0,     0,   135,    83,
       0,     0,     0,     0,     0,     0,     0,   138,     0,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,   115,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,     0,    56,     0,     0,
       0,     0,     0,     0,   125,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,     0,
       0,     0,     0,     0,     0,     0,   103,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,     0,
       0,     0,     0,   102,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    85,     0,     0,    59,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,   123,     0,     0,     0,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47
};

static const yytype_int16 yycheck[] =
{
       0,     3,   109,     8,    16,     5,     6,     7,     3,     8,
      56,     3,    14,    19,    20,    15,    16,    29,     0,    65,
      11,    10,    21,    12,    29,    14,   133,    11,    30,    44,
      45,    46,    29,     3,     3,     3,    39,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
       3,     4,     5,    46,    44,     3,    11,    10,    11,    59,
      60,    61,    21,    15,    10,    18,    41,    21,    12,     3,
       3,     8,    11,     3,     4,     5,    40,     3,    31,    29,
      10,    34,    35,    83,    41,    38,    86,    15,    18,    42,
      43,     3,     7,     3,    94,    30,    21,    29,    98,   115,
     100,    31,   102,   103,    34,    35,    33,    83,    38,    65,
      -1,    -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,   125,    -1,    -1,   128,     7,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,     7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    -1,     9,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    36,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    -1,
      -1,    -1,    -1,    33,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    13,    -1,    -1,    32,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    13,    -1,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,    10,    18,    31,    34,    35,    38,
      42,    43,    49,    59,    60,    10,    12,    14,    60,    63,
      64,    60,    60,    60,     3,    44,    45,    46,    50,    51,
      52,    53,    56,    57,    58,     0,    16,    29,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      11,    60,    61,    60,     3,    62,     9,    11,    11,    32,
      37,    29,     3,     3,     3,    39,    50,    53,    58,     3,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,     7,    11,    13,    21,    15,    64,    60,
      60,    60,    10,     8,    29,    21,    63,    64,    12,    61,
      41,    60,    33,    36,     3,    55,     3,    60,     3,    14,
      30,    54,    40,    60,    60,     7,    60,    60,     8,    11,
      29,    55,    41,    13,    62,    37,     3,     8,    21,    60,
      15,     3,    60,     7,     3,    60,    55,    21,    60
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    48,    49,    50,    50,    51,    51,    51,    52,    53,
      53,    54,    54,    54,    55,    55,    55,    56,    56,    57,
      57,    58,    58,    59,    59,    59,    59,    60,    60,    60,
      60,    60,    60,    60,    60,    61,    61,    61,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    62,    62,    62,    60,    60,    60,    60,    60,
      60,    60,    63,    64,    64,    64,    60
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     4,     1,
       2,     1,     3,     3,     0,     3,     5,     4,     6,     7,
       9,     1,     2,     1,     3,     4,     6,     1,     1,     1,
       3,     1,     1,     3,     4,     0,     1,     3,     3,     3,
       3,     3,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     4,     0,     3,     5,     6,     6,     4,     3,     4,
       8,     5,     1,     0,     1,     3,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
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
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
        case 2:
#line 94 "tiger.y" /* yacc.c:1652  */
    {absyn_root=(yyvsp[0].exp);}
#line 1472 "y.tab.c" /* yacc.c:1652  */
    break;

  case 3:
#line 100 "tiger.y" /* yacc.c:1652  */
    { (yyval.decs)=A_DecList((yyvsp[-1].dec),(yyvsp[0].decs));}
#line 1478 "y.tab.c" /* yacc.c:1652  */
    break;

  case 4:
#line 101 "tiger.y" /* yacc.c:1652  */
    { (yyval.decs)=A_DecList((yyvsp[0].dec), NULL);}
#line 1484 "y.tab.c" /* yacc.c:1652  */
    break;

  case 5:
#line 103 "tiger.y" /* yacc.c:1652  */
    { (yyval.dec)=A_TypeDec(tokPos, (yyvsp[0].nametys));}
#line 1490 "y.tab.c" /* yacc.c:1652  */
    break;

  case 6:
#line 104 "tiger.y" /* yacc.c:1652  */
    { (yyval.dec)=(yyvsp[0].dec);}
#line 1496 "y.tab.c" /* yacc.c:1652  */
    break;

  case 7:
#line 105 "tiger.y" /* yacc.c:1652  */
    { (yyval.dec)=A_FunctionDec(tokPos, (yyvsp[0].fundecs));}
#line 1502 "y.tab.c" /* yacc.c:1652  */
    break;

  case 8:
#line 108 "tiger.y" /* yacc.c:1652  */
    { (yyval.namety)=A_Namety(S_Symbol((yyvsp[-2].sval)), (yyvsp[0].ty));}
#line 1508 "y.tab.c" /* yacc.c:1652  */
    break;

  case 9:
#line 110 "tiger.y" /* yacc.c:1652  */
    { (yyval.nametys)=A_NametyList((yyvsp[0].namety),NULL);}
#line 1514 "y.tab.c" /* yacc.c:1652  */
    break;

  case 10:
#line 111 "tiger.y" /* yacc.c:1652  */
    { (yyval.nametys)=A_NametyList((yyvsp[-1].namety), (yyvsp[0].nametys));}
#line 1520 "y.tab.c" /* yacc.c:1652  */
    break;

  case 11:
#line 113 "tiger.y" /* yacc.c:1652  */
    { (yyval.ty)=A_NameTy(tokPos, S_Symbol((yyvsp[0].sval)));}
#line 1526 "y.tab.c" /* yacc.c:1652  */
    break;

  case 12:
#line 114 "tiger.y" /* yacc.c:1652  */
    { (yyval.ty)=A_RecordTy(tokPos, (yyvsp[-1].fields));}
#line 1532 "y.tab.c" /* yacc.c:1652  */
    break;

  case 13:
#line 115 "tiger.y" /* yacc.c:1652  */
    { (yyval.ty)=A_ArrayTy(tokPos, S_Symbol((yyvsp[0].sval)));}
#line 1538 "y.tab.c" /* yacc.c:1652  */
    break;

  case 14:
#line 117 "tiger.y" /* yacc.c:1652  */
    {(yyval.fields)=NULL;}
#line 1544 "y.tab.c" /* yacc.c:1652  */
    break;

  case 15:
#line 118 "tiger.y" /* yacc.c:1652  */
    {(yyval.fields)=A_FieldList(A_Field(tokPos, S_Symbol((yyvsp[-2].sval)), S_Symbol((yyvsp[0].sval))), NULL);}
#line 1550 "y.tab.c" /* yacc.c:1652  */
    break;

  case 16:
#line 120 "tiger.y" /* yacc.c:1652  */
    {(yyval.fields)=A_FieldList(A_Field(tokPos, S_Symbol((yyvsp[-4].sval)), S_Symbol((yyvsp[-2].sval))), (yyvsp[0].fields));}
#line 1556 "y.tab.c" /* yacc.c:1652  */
    break;

  case 17:
#line 124 "tiger.y" /* yacc.c:1652  */
    {(yyval.dec)=A_VarDec(tokPos, S_Symbol((yyvsp[-2].sval)), NULL, (yyvsp[0].exp));}
#line 1562 "y.tab.c" /* yacc.c:1652  */
    break;

  case 18:
#line 125 "tiger.y" /* yacc.c:1652  */
    {(yyval.dec)=A_VarDec(tokPos, S_Symbol((yyvsp[-4].sval)), S_Symbol((yyvsp[-2].sval)), (yyvsp[0].exp));}
#line 1568 "y.tab.c" /* yacc.c:1652  */
    break;

  case 19:
#line 129 "tiger.y" /* yacc.c:1652  */
    {(yyval.fundec)= A_Fundec(tokPos, S_Symbol((yyvsp[-5].sval)), (yyvsp[-3].fields), NULL, (yyvsp[0].exp));}
#line 1574 "y.tab.c" /* yacc.c:1652  */
    break;

  case 20:
#line 131 "tiger.y" /* yacc.c:1652  */
    {(yyval.fundec)= A_Fundec(tokPos, S_Symbol((yyvsp[-7].sval)), (yyvsp[-5].fields), S_Symbol((yyvsp[-2].sval)), (yyvsp[0].exp));}
#line 1580 "y.tab.c" /* yacc.c:1652  */
    break;

  case 21:
#line 133 "tiger.y" /* yacc.c:1652  */
    {(yyval.fundecs)=A_FundecList((yyvsp[0].fundec),NULL);}
#line 1586 "y.tab.c" /* yacc.c:1652  */
    break;

  case 22:
#line 134 "tiger.y" /* yacc.c:1652  */
    {(yyval.fundecs)=A_FundecList((yyvsp[-1].fundec), (yyvsp[0].fundecs));}
#line 1592 "y.tab.c" /* yacc.c:1652  */
    break;

  case 23:
#line 140 "tiger.y" /* yacc.c:1652  */
    {(yyval.var)=A_SimpleVar(tokPos, S_Symbol((yyvsp[0].sval)));}
#line 1598 "y.tab.c" /* yacc.c:1652  */
    break;

  case 24:
#line 141 "tiger.y" /* yacc.c:1652  */
    {(yyval.var)=A_FieldVar(tokPos, (yyvsp[-2].var), S_Symbol((yyvsp[0].sval)));}
#line 1604 "y.tab.c" /* yacc.c:1652  */
    break;

  case 25:
#line 149 "tiger.y" /* yacc.c:1652  */
    {(yyval.var)=A_SubscriptVar(tokPos, 
   A_SimpleVar(tokPos, S_Symbol((yyvsp[-3].sval))), (yyvsp[-1].exp));}
#line 1611 "y.tab.c" /* yacc.c:1652  */
    break;

  case 26:
#line 151 "tiger.y" /* yacc.c:1652  */
    {(yyval.var)=A_SubscriptVar(tokPos, 
   A_FieldVar(tokPos, (yyvsp[-5].var), S_Symbol((yyvsp[-3].sval))), (yyvsp[-1].exp));}
#line 1618 "y.tab.c" /* yacc.c:1652  */
    break;

  case 27:
#line 160 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_VarExp(tokPos, (yyvsp[0].var));}
#line 1624 "y.tab.c" /* yacc.c:1652  */
    break;

  case 28:
#line 165 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_BreakExp(tokPos);}
#line 1630 "y.tab.c" /* yacc.c:1652  */
    break;

  case 29:
#line 168 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_NilExp(tokPos);}
#line 1636 "y.tab.c" /* yacc.c:1652  */
    break;

  case 30:
#line 170 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_SeqExp(tokPos, (yyvsp[-1].exps));}
#line 1642 "y.tab.c" /* yacc.c:1652  */
    break;

  case 31:
#line 174 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_IntExp(tokPos, (yyvsp[0].ival));}
#line 1648 "y.tab.c" /* yacc.c:1652  */
    break;

  case 32:
#line 177 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_StringExp(tokPos, (yyvsp[0].sval));}
#line 1654 "y.tab.c" /* yacc.c:1652  */
    break;

  case 33:
#line 180 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_CallExp(tokPos, S_Symbol((yyvsp[-2].sval)), NULL);}
#line 1660 "y.tab.c" /* yacc.c:1652  */
    break;

  case 34:
#line 181 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_CallExp(tokPos, S_Symbol((yyvsp[-3].sval)), (yyvsp[-1].exps));}
#line 1666 "y.tab.c" /* yacc.c:1652  */
    break;

  case 35:
#line 183 "tiger.y" /* yacc.c:1652  */
    {(yyval.exps)=NULL;}
#line 1672 "y.tab.c" /* yacc.c:1652  */
    break;

  case 36:
#line 184 "tiger.y" /* yacc.c:1652  */
    {(yyval.exps)=A_ExpList((yyvsp[0].exp), NULL);}
#line 1678 "y.tab.c" /* yacc.c:1652  */
    break;

  case 37:
#line 185 "tiger.y" /* yacc.c:1652  */
    {(yyval.exps)=A_ExpList((yyvsp[-2].exp), (yyvsp[0].exps));}
#line 1684 "y.tab.c" /* yacc.c:1652  */
    break;

  case 38:
#line 188 "tiger.y" /* yacc.c:1652  */
    { (yyval.exp)=A_OpExp(tokPos, A_plusOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1690 "y.tab.c" /* yacc.c:1652  */
    break;

  case 39:
#line 189 "tiger.y" /* yacc.c:1652  */
    { (yyval.exp)=A_OpExp(tokPos, A_minusOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1696 "y.tab.c" /* yacc.c:1652  */
    break;

  case 40:
#line 190 "tiger.y" /* yacc.c:1652  */
    { (yyval.exp)=A_OpExp(tokPos, A_timesOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1702 "y.tab.c" /* yacc.c:1652  */
    break;

  case 41:
#line 191 "tiger.y" /* yacc.c:1652  */
    { (yyval.exp)=A_OpExp(tokPos, A_divideOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1708 "y.tab.c" /* yacc.c:1652  */
    break;

  case 42:
#line 192 "tiger.y" /* yacc.c:1652  */
    { (yyval.exp)=A_OpExp(tokPos, A_minusOp, A_IntExp(0, 0), (yyvsp[0].exp));}
#line 1714 "y.tab.c" /* yacc.c:1652  */
    break;

  case 43:
#line 195 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_OpExp(tokPos, A_eqOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1720 "y.tab.c" /* yacc.c:1652  */
    break;

  case 44:
#line 196 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_OpExp(tokPos, A_neqOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1726 "y.tab.c" /* yacc.c:1652  */
    break;

  case 45:
#line 197 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_OpExp(tokPos, A_gtOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1732 "y.tab.c" /* yacc.c:1652  */
    break;

  case 46:
#line 198 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_OpExp(tokPos, A_ltOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1738 "y.tab.c" /* yacc.c:1652  */
    break;

  case 47:
#line 199 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_OpExp(tokPos, A_geOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1744 "y.tab.c" /* yacc.c:1652  */
    break;

  case 48:
#line 200 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_OpExp(tokPos, A_leOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1750 "y.tab.c" /* yacc.c:1652  */
    break;

  case 49:
#line 204 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_IfExp(tokPos, (yyvsp[-2].exp), (yyvsp[0].exp), A_IntExp(tokPos, 0));}
#line 1756 "y.tab.c" /* yacc.c:1652  */
    break;

  case 50:
#line 205 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_IfExp(tokPos, (yyvsp[-2].exp), A_IntExp(tokPos, 1), (yyvsp[0].exp));}
#line 1762 "y.tab.c" /* yacc.c:1652  */
    break;

  case 51:
#line 208 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_RecordExp(tokPos, S_Symbol((yyvsp[-3].sval)), (yyvsp[-1].efields));}
#line 1768 "y.tab.c" /* yacc.c:1652  */
    break;

  case 52:
#line 210 "tiger.y" /* yacc.c:1652  */
    {(yyval.efields)=NULL;}
#line 1774 "y.tab.c" /* yacc.c:1652  */
    break;

  case 53:
#line 211 "tiger.y" /* yacc.c:1652  */
    {(yyval.efields)=A_EfieldList(A_Efield(S_Symbol((yyvsp[-2].sval)), (yyvsp[0].exp)), NULL);}
#line 1780 "y.tab.c" /* yacc.c:1652  */
    break;

  case 54:
#line 212 "tiger.y" /* yacc.c:1652  */
    {(yyval.efields)=A_EfieldList(A_Efield(S_Symbol((yyvsp[-4].sval)), (yyvsp[-2].exp)), (yyvsp[0].efields));}
#line 1786 "y.tab.c" /* yacc.c:1652  */
    break;

  case 55:
#line 215 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_ArrayExp(tokPos, S_Symbol((yyvsp[-5].sval)), (yyvsp[-3].exp), (yyvsp[0].exp));}
#line 1792 "y.tab.c" /* yacc.c:1652  */
    break;

  case 56:
#line 218 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_IfExp(tokPos, (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1798 "y.tab.c" /* yacc.c:1652  */
    break;

  case 57:
#line 221 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_IfExp(tokPos, (yyvsp[-2].exp), (yyvsp[0].exp), NULL);}
#line 1804 "y.tab.c" /* yacc.c:1652  */
    break;

  case 58:
#line 224 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_AssignExp(tokPos, (yyvsp[-2].var), (yyvsp[0].exp));}
#line 1810 "y.tab.c" /* yacc.c:1652  */
    break;

  case 59:
#line 227 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_WhileExp(tokPos, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1816 "y.tab.c" /* yacc.c:1652  */
    break;

  case 60:
#line 231 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_ForExp(tokPos, S_Symbol((yyvsp[-6].sval)), (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1822 "y.tab.c" /* yacc.c:1652  */
    break;

  case 61:
#line 234 "tiger.y" /* yacc.c:1652  */
    {
		(yyval.exp)=A_LetExp(tokPos, (yyvsp[-3].decs), (yyvsp[-1].exp));
   }
#line 1830 "y.tab.c" /* yacc.c:1652  */
    break;

  case 62:
#line 238 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=A_SeqExp(tokPos, (yyvsp[0].exps));}
#line 1836 "y.tab.c" /* yacc.c:1652  */
    break;

  case 63:
#line 240 "tiger.y" /* yacc.c:1652  */
    {(yyval.exps)=NULL;}
#line 1842 "y.tab.c" /* yacc.c:1652  */
    break;

  case 64:
#line 241 "tiger.y" /* yacc.c:1652  */
    {(yyval.exps)=A_ExpList((yyvsp[0].exp), NULL);}
#line 1848 "y.tab.c" /* yacc.c:1652  */
    break;

  case 65:
#line 242 "tiger.y" /* yacc.c:1652  */
    {(yyval.exps)=A_ExpList((yyvsp[-2].exp), (yyvsp[0].exps));}
#line 1854 "y.tab.c" /* yacc.c:1652  */
    break;

  case 66:
#line 245 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp)=(yyvsp[-1].exp);}
#line 1860 "y.tab.c" /* yacc.c:1652  */
    break;


#line 1864 "y.tab.c" /* yacc.c:1652  */
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

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
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
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
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
