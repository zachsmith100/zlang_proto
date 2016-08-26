/* A Bison parser, made by GNU Bison 3.0.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "yacc.y" /* yacc.c:339  */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "node.h"

#ifdef WIN32
void _strcat(char* str1, const char* str2)
{
	strcat_s(str1, 1000, str2);
}

	#define strdup _strdup
	#define strcat _strcat
#endif

extern int yylineno; 

int errorLine = 0;
char* errorText = NULL;

void yyerror(const char* str)
{
	errorLine = yylineno;
	errorText = strdup(str);
//        fprintf(stderr, "Fatal error at line %d: %s\n", yylineno, str);
}

extern int yylex(void);

node* root = NULL;

node* getroot()
{
	return root;
}

char* getYaccErrorText()
{
	return errorText;
}

int getYaccErrorLine()
{
	return errorLine;
}


extern "C" int yywrap()
{
        return 1;
} 

//#define PRINT_CREATING_NODES_INFO 1
  
node* mknode(node* left, node* center, node* right, const char* token)
{
	node* newnode = (node *)malloc(sizeof(node));
	if (token != NULL)
	{
		char* newStr = strdup(token);
		newnode->token = newStr;
	}
	else
	{
		newnode->token = NULL;
	}

	newnode->left = left;
	newnode->right = right;
	newnode->center = center;
	newnode->nodeval = NULL;
	newnode->cmd = -1;
	newnode->processed = 0;
	newnode->line = yylineno;

#ifdef PRINT_CREATING_NODES_INFO
	if (newnode->token != NULL)
	{
		printf("#%d: mknode token: %s nodes: %d %d %d\n", (int)newnode, newnode->token, (int)left, (int)center, (int)right);
	}
#endif

	return newnode;
}

node* mkcmdnode(node* left, node* center, node* right, int cmd)
{
	node* newnode = (node *)malloc(sizeof(node));
	newnode->token = NULL;
	newnode->cmd = cmd;
	newnode->left = left;
	newnode->right = right;
	newnode->center = center;
	newnode->nodeval = NULL;
	newnode->processed = 0;
	newnode->line = yylineno;

#ifdef PRINT_CREATING_NODES_INFO
	printf("#%d: mkcmdnode cmd: %d nodes: %d %d %d\n", (int)newnode, cmd, (int)left, (int)center, (int)right);
#endif

	return newnode;
}

extern int yylex(void);

#line 174 "yacc.tab.c" /* yacc.c:339  */

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "yacc.tab.h".  */
#ifndef YY_YY_YACC_TAB_H_INCLUDED
# define YY_YY_YACC_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    HEX_NUMBER = 258,
    FP_NUMBER = 259,
    NUMBER = 260,
    ID_FUNC = 261,
    ID_CLASS = 262,
    WHILE = 263,
    IF = 264,
    FOR = 265,
    OR = 266,
    AND = 267,
    NEWLINE = 268,
    CODE_LEVEL = 269,
    SQUARE_BRACKET_START = 270,
    SQUARE_BRACKET_END = 271,
    OPERATOR_EQUAL = 272,
    OPERATOR_INCREMENT = 273,
    OPERATOR_DECREMENT = 274,
    OPERATOR_MINUS = 275,
    OPERATOR_PLUS = 276,
    OPERATOR_MULTIPLY = 277,
    OPERATOR_DIVISION = 278,
    COMPARE_LESS = 279,
    COMPARE_MORE = 280,
    COMPARE_EQUAL = 281,
    COMPARE_NOT_EQUAL = 282,
    CONDITION_OR = 283,
    END_PART = 284,
    SYMBOL = 285,
    COMMA = 286,
    VAL_TRUE = 287,
    VAL_FALSE = 288,
    OPERATOR_RETURN = 289,
    PARENS_START = 290,
    PARENS_END = 291,
    OPERATOR_PLUS_EQUAL = 292,
    OPERATOR_MINUS_EQUAL = 293,
    OPERATOR_MULTIPLY_EQUAL = 294,
    OPERATOR_DIVISION_EQUAL = 295,
    COMPARE_LESS_EQUAL = 296,
    COMPARE_MORE_EQUAL = 297,
    STRING = 298,
    PRIVATE = 299,
    PUBLIC = 300,
    SINGLETON = 301,
    CODE_LEVEL3 = 302,
    DOT = 303,
    ME = 304,
    OPERATOR_CONCAT = 305,
    BASE = 306,
    OUT_SPEC = 307,
    MULTILINE_COMMENT = 308,
    SINGLELINE_COMMENT = 309,
    ELSE = 310,
    FOREACH = 311,
    IN_CONSTRUCT = 312,
    CPP = 313,
    UNIX_IMPORT = 314,
    WINDOWS_IMPORT = 315,
    LINUX_IMPORT = 316,
    MACOSX_IMPORT = 317,
    INCLUDE = 318,
    LINUX_INCLUDE = 319,
    WINDOWS_INCLUDE = 320,
    MACOSX_INCLUDE = 321,
    UNIX_INCLUDE = 322,
    UNIX_IMPORT_PATH = 323,
    WINDOWS_IMPORT_PATH = 324,
    LINUX_IMPORT_PATH = 325,
    MACOSX_IMPORT_PATH = 326,
    WINDOWS_INCLUDE_PATH = 327,
    UNIX_INCLUDE_PATH = 328,
    LINUX_INCLUDE_PATH = 329,
    MACOSX_INCLUDE_PATH = 330,
    ADD_CPP = 331,
    BREAK = 332,
    CONTINUE = 333,
    QUESTION_SIGN = 334,
    COLON = 335,
    DOG = 336,
    CPP_INHERIT = 337,
    OBJECT = 338,
    INCLUDE_PATH = 339,
    UNIX_COMPILER_FLAGS = 340,
    LINUX_COMPILER_FLAGS = 341,
    MACOSX_COMPILER_FLAGS = 342,
    CPP_ENTRY_POINT = 343,
    OPERATOR_MATH_AND = 344,
    OPERATOR_MATH_OR = 345,
    OPERATOR_MATH_XOR = 346,
    OPERATOR_AND_EQUAL = 347,
    OPERATOR_OR_EQUAL = 348,
    OPERATOR_XOR_EQUAL = 349,
    FP_ARB_NUMBER = 350,
    FLOAT_DECL = 351,
    FIXED_DECL = 352
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 113 "yacc.y" /* yacc.c:355  */

  int intval;
  char* strval;
  struct node* nodeval;  

#line 318 "yacc.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_YACC_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 333 "yacc.tab.c" /* yacc.c:358  */

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
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
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
#  define YYSIZE_T unsigned int
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

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
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
#define YYFINAL  85
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1230

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  103
/* YYNRULES -- Number of rules.  */
#define YYNRULES  273
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  441

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   352

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   125,   125,   129,   131,   133,   135,   139,   141,   143,
     145,   147,   149,   151,   153,   155,   157,   161,   163,   165,
     167,   169,   173,   175,   177,   179,   181,   185,   187,   189,
     191,   195,   197,   199,   201,   203,   207,   209,   211,   215,
     219,   223,   227,   229,   231,   233,   235,   239,   243,   245,
     249,   251,   255,   257,   259,   263,   265,   269,   271,   275,
     277,   281,   285,   287,   289,   291,   293,   295,   297,   301,
     303,   307,   309,   311,   313,   317,   319,   323,   327,   331,
     333,   335,   337,   341,   343,   345,   347,   349,   353,   355,
     357,   361,   363,   365,   369,   373,   375,   377,   379,   381,
     383,   385,   387,   389,   391,   395,   397,   399,   401,   403,
     407,   409,   413,   415,   419,   421,   425,   427,   431,   433,
     437,   439,   443,   445,   447,   451,   455,   457,   461,   463,
     465,   469,   471,   473,   475,   479,   481,   485,   487,   489,
     491,   493,   497,   499,   503,   505,   509,   511,   515,   517,
     519,   523,   525,   527,   529,   531,   533,   535,   539,   541,
     545,   547,   551,   555,   559,   563,   567,   571,   575,   577,
     579,   581,   583,   585,   589,   593,   595,   597,   599,   601,
     605,   607,   609,   611,   613,   615,   617,   619,   621,   623,
     627,   629,   631,   633,   637,   641,   643,   647,   651,   655,
     657,   659,   663,   665,   667,   669,   671,   673,   675,   677,
     679,   681,   683,   687,   689,   691,   693,   695,   697,   701,
     703,   707,   709,   711,   713,   715,   717,   719,   721,   723,
     727,   729,   733,   737,   739,   743,   747,   751,   755,   759,
     763,   767,   769,   773,   775,   779,   783,   785,   789,   791,
     795,   797,   801,   805,   807,   811,   815,   819,   823,   827,
     831,   835,   837,   841,   843,   847,   849,   853,   855,   859,
     861,   863,   867,   869
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "HEX_NUMBER", "FP_NUMBER", "NUMBER",
  "ID_FUNC", "ID_CLASS", "WHILE", "IF", "FOR", "OR", "AND", "NEWLINE",
  "CODE_LEVEL", "SQUARE_BRACKET_START", "SQUARE_BRACKET_END",
  "OPERATOR_EQUAL", "OPERATOR_INCREMENT", "OPERATOR_DECREMENT",
  "OPERATOR_MINUS", "OPERATOR_PLUS", "OPERATOR_MULTIPLY",
  "OPERATOR_DIVISION", "COMPARE_LESS", "COMPARE_MORE", "COMPARE_EQUAL",
  "COMPARE_NOT_EQUAL", "CONDITION_OR", "END_PART", "SYMBOL", "COMMA",
  "VAL_TRUE", "VAL_FALSE", "OPERATOR_RETURN", "PARENS_START", "PARENS_END",
  "OPERATOR_PLUS_EQUAL", "OPERATOR_MINUS_EQUAL", "OPERATOR_MULTIPLY_EQUAL",
  "OPERATOR_DIVISION_EQUAL", "COMPARE_LESS_EQUAL", "COMPARE_MORE_EQUAL",
  "STRING", "PRIVATE", "PUBLIC", "SINGLETON", "CODE_LEVEL3", "DOT", "ME",
  "OPERATOR_CONCAT", "BASE", "OUT_SPEC", "MULTILINE_COMMENT",
  "SINGLELINE_COMMENT", "ELSE", "FOREACH", "IN_CONSTRUCT", "CPP",
  "UNIX_IMPORT", "WINDOWS_IMPORT", "LINUX_IMPORT", "MACOSX_IMPORT",
  "INCLUDE", "LINUX_INCLUDE", "WINDOWS_INCLUDE", "MACOSX_INCLUDE",
  "UNIX_INCLUDE", "UNIX_IMPORT_PATH", "WINDOWS_IMPORT_PATH",
  "LINUX_IMPORT_PATH", "MACOSX_IMPORT_PATH", "WINDOWS_INCLUDE_PATH",
  "UNIX_INCLUDE_PATH", "LINUX_INCLUDE_PATH", "MACOSX_INCLUDE_PATH",
  "ADD_CPP", "BREAK", "CONTINUE", "QUESTION_SIGN", "COLON", "DOG",
  "CPP_INHERIT", "OBJECT", "INCLUDE_PATH", "UNIX_COMPILER_FLAGS",
  "LINUX_COMPILER_FLAGS", "MACOSX_COMPILER_FLAGS", "CPP_ENTRY_POINT",
  "OPERATOR_MATH_AND", "OPERATOR_MATH_OR", "OPERATOR_MATH_XOR",
  "OPERATOR_AND_EQUAL", "OPERATOR_OR_EQUAL", "OPERATOR_XOR_EQUAL",
  "FP_ARB_NUMBER", "FLOAT_DECL", "FIXED_DECL", "$accept", "program",
  "program_parts", "program_part", "import_directive", "include_directive",
  "import_path_directive", "include_path_directive",
  "compiler_flags_directive", "entry_point_directive", "add_cpp_directive",
  "class", "class_decl", "class_type", "section_exp", "section_type",
  "functions_and_properties", "function_parameters_annotation",
  "global_function", "global_function_impl", "function_or_property", "fps",
  "property", "newlines", "function", "function_commands",
  "global_function_commands", "parameters", "var", "commands",
  "global_func_commands", "command", "cmd", "function_call",
  "prop_method_call", "internal_method_call", "var_method_call",
  "standard_function_call", "base_method_call", "props_in_call",
  "function_call_parameters_with_parens", "object_exp",
  "function_call_parameters", "code_levels", "global_code_levels",
  "math_unary_exp", "command_simple", "return_props", "array_element_var",
  "array_element", "making_equal_simple", "operator_change_value",
  "left_equal_part", "id_exp", "id_func_exp", "id_class_exp",
  "code_level_exp", "code_level3_exp", "number_exp",
  "fp_simple_number_exp", "fp_number_exp", "hex_number_exp",
  "string_right_part", "right_equal_part_min", "right_equal_part",
  "constructor", "constructor_params", "constructor_parameter",
  "string_exp", "plus_exp", "math_exp", "operand", "boolean_var",
  "condition", "condition_operator", "if_operator", "one_line_if_operator",
  "else_operator", "break_operator", "continue_operator", "while_operator",
  "for_operator", "comma_commands_for", "comma_commands",
  "initializations", "func_result_prop_exp", "my_prop_exp",
  "base_prop_exp", "prop_exp", "out_var_exp", "comment",
  "singleline_comment_exp", "multiline_comment_exp", "foreach_operator",
  "cpp_declaration", "cpp_operator", "cpp_exp",
  "method_parameter_annotation", "method_annotation_properties",
  "function_parameter_annotation", "function_annotation_properties",
  "annotation_value", "annotation_values", YY_NULL
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352
};
# endif

#define YYPACT_NINF -359

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-359)))

#define YYTABLE_NINF -222

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     993,  -359,  -359,  -359,  -359,  -359,  -359,  -359,    20,    20,
      20,    20,    20,    20,    20,    20,    20,    20,    20,    20,
      20,    20,    20,    20,    20,    20,   159,    20,    20,    20,
      20,    20,    88,   993,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,   133,    99,    22,  -359,  -359,  1069,   354,
    -359,    44,  -359,  -359,   133,  -359,   133,  -359,  -359,  -359,
      79,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,  -359,   133,   133,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,  -359,  1069,   404,    37,
    -359,  -359,  -359,  -359,  -359,   193,   259,  -359,   354,  -359,
    -359,    12,   209,   209,    20,   404,   404,  -359,  -359,   428,
    -359,   520,    99,  -359,  -359,   312,  1152,  -359,  -359,  -359,
    -359,  -359,    92,  -359,  -359,   193,  -359,  -359,  -359,   404,
     133,   133,  -359,  -359,    62,  -359,  -359,  1152,   610,   879,
     879,   168,   879,    73,    73,   133,   193,   133,   133,   415,
    -359,  -359,    73,  -359,  -359,  -359,  -359,  -359,  -359,   133,
     108,    33,  -359,    -1,    32,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,    73,  -359,    73,  -359,  -359,  -359,
     285,    49,  -359,   209,   209,   274,  -359,  -359,   124,   354,
    -359,  -359,   133,  -359,  -359,  -359,  -359,  -359,  -359,   417,
    -359,  -359,   879,  -359,   154,   167,   161,    11,  -359,   365,
     690,  -359,   748,   826,   571,   146,   173,   925,  -359,  -359,
      86,    86,   194,    73,  -359,    26,   263,   193,    60,    84,
     209,   131,   209,   209,  -359,  -359,   170,   209,   651,   170,
     888,  -359,  -359,  -359,  -359,   249,  -359,  -359,  -359,   888,
     702,   160,  -359,   255,   255,  -359,  -359,   193,   440,   133,
     133,   888,   143,  -359,   354,   209,   532,   217,    75,   252,
     252,   937,   937,   937,   937,   937,   937,   947,   947,   947,
     947,   947,   947,   947,  -359,  -359,   209,   879,   209,   879,
     168,   193,  -359,   724,   193,  -359,   758,  -359,   193,  -359,
     888,   261,  1072,   245,  -359,    34,  -359,  -359,  -359,   194,
      73,    33,  -359,  -359,  -359,   220,   780,  -359,   829,  -359,
     838,  -359,  -359,   242,   193,  -359,  -359,   133,  -359,   143,
     143,   341,   133,   437,  1152,  -359,  -359,  -359,  -359,  -359,
     246,   257,   947,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
     415,    73,    33,  -359,  -359,  -359,  -359,   173,   939,   925,
     925,   925,   925,   925,   925,   330,   323,  -359,   280,  -359,
     220,  -359,   220,   133,    71,   286,   702,  -359,   888,   265,
     290,  -359,   220,   220,  -359,  -359,   404,   193,  -359,  -359,
     209,   209,  1152,   610,  -359,  -359,  -359,   309,   311,   935,
     164,   193,   209,  -359,   -14,   127,  -359,   270,  -359,  -359,
     265,  -359,   277,  -359,  -359,   947,   947,  -359,  -359,   362,
    -359,   937,   193,  -359,   888,  -359,   440,   981,   999,   164,
     209,  -359,   278,  -359,  -359,   133,  -359,  -359,  -359,   143,
    -359
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,   162,   163,    71,    47,   256,   255,   260,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     2,     3,     9,    10,    11,    12,    13,
      15,    14,     7,     0,     0,     0,     8,    57,     0,     0,
     160,    42,    72,   253,     0,    16,     0,    55,   198,    17,
      18,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    40,     0,     0,   161,
      35,    37,    38,    36,    39,     1,     5,     0,     0,    45,
      58,    56,    73,     4,    74,     0,     0,    60,     0,    79,
      80,     0,   254,   259,     0,     0,     0,     6,   164,    41,
      52,     0,     0,   252,   165,    78,     0,   131,   132,    59,
      81,    82,     0,    43,    19,     0,   266,   265,    53,     0,
       0,     0,    64,    61,     0,    65,    46,     0,     0,     0,
       0,     0,     0,   247,   249,     0,     0,     0,     0,   158,
      91,    94,   141,   105,   106,   107,   108,   109,   137,     0,
       0,   159,   138,     0,    83,   133,   134,    95,    96,   100,
     101,    97,    98,    87,    84,    85,    86,   103,    99,   104,
       0,     0,    54,    50,    51,     0,    63,    62,     0,   160,
      66,    68,     0,    67,    92,    93,   174,   167,   166,   188,
     219,   220,     0,   169,     0,     0,   185,   182,   202,   186,
     181,   168,   183,   184,   221,   187,     0,   180,   203,   189,
       0,     0,   158,     0,   243,     0,   139,     0,   246,   248,
     235,     0,   236,   237,   135,   136,   245,   102,     0,   251,
       0,   151,   152,   153,   154,     0,   155,   156,   157,     0,
       0,   250,   116,     0,     0,    48,    49,     0,     0,     0,
       0,     0,     0,    76,     0,   258,   180,   203,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   230,   231,   238,     0,   232,     0,
       0,     0,   120,     0,     0,   112,     0,   118,     0,   147,
       0,    83,   191,     0,   193,   192,   190,   148,   178,   175,
     179,   176,   150,   177,   149,   117,     0,   126,     0,   114,
       0,   110,   111,     0,   272,   270,   269,     0,   271,     0,
       0,   182,     0,    77,     0,   128,    75,   205,   204,   229,
       0,     0,     0,   225,   226,   223,   224,   227,   228,   216,
     217,   201,   218,   213,   214,   215,   200,   207,   199,   206,
     208,   209,   210,   211,   212,   222,     0,   244,   140,   142,
     113,   121,   119,     0,     0,     0,     0,   146,     0,     0,
     122,   127,   115,     0,    44,   273,   267,     0,   262,   261,
      70,    69,     0,     0,    88,   129,   130,   171,   173,   199,
       0,     0,   257,   234,    83,     0,   195,     0,   125,   124,
       0,   268,     0,    89,    90,     0,     0,   241,   239,     0,
     143,     0,     0,   194,     0,   123,     0,   199,   199,     0,
     240,   197,     0,   196,   233,     0,   170,   172,   242,   263,
     264
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -359,  -359,  -359,    57,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,   319,
     -25,  -359,  -359,     0,  -359,   104,   271,   183,   233,  -359,
    -359,  -123,  -359,   -76,  -359,  -359,  -359,  -359,  -359,   235,
     248,   -33,  -359,    48,   269,   -37,  -358,  -359,  -359,   260,
    -136,  -359,  -359,    27,   266,    41,   -93,   -85,   300,  -359,
     144,   328,  -359,  -138,  -214,  -359,  -359,   -31,   215,   125,
     188,  -182,  -359,  -132,  -359,  -359,   110,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,   -92,
     199,  -359,  -359,  -359,  -359,    25,   314,  -359,  -313,   359,
     -97,    -6,   119
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,   257,   132,   109,    45,    46,    47,
     110,   133,   186,    96,   187,   263,    97,    98,   206,   333,
     115,   150,   151,   207,   153,   154,   155,   156,   157,   236,
     252,   409,   316,   334,   116,   208,   159,   368,   160,   209,
     162,   249,   163,   164,    50,    79,   111,   387,   210,   211,
     212,   213,   312,   302,   317,   304,   405,   406,   215,   216,
     217,   218,   219,   305,   287,   167,   306,   168,   169,   170,
     171,   172,   418,   419,   225,   173,   174,   175,   176,   100,
      52,    53,    54,   178,   191,   179,    56,   193,   388,    57,
     126,   327,   328
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   214,   214,   117,   214,   224,   121,   220,   221,   127,
     226,   118,   125,   125,   194,   195,   240,   389,   134,     2,
     267,   376,   117,   165,   303,    55,   307,    49,     1,     2,
     118,   166,  -201,    87,   227,   314,   241,   242,   243,   244,
     152,    51,   417,    88,   165,   284,   285,   332,  -145,   245,
    -161,  -161,   166,    78,   102,   289,   103,   290,    55,   227,
      49,   152,   152,    58,   214,   223,   421,   250,     1,     2,
     268,   438,    49,    55,    51,    49,    99,   105,   106,   158,
     227,   227,   284,   285,   128,    89,   284,   285,    85,    51,
      86,   246,   247,   248,   122,   293,  -161,   284,   285,     3,
     158,   158,   381,    26,   182,    93,     2,   339,   294,   129,
     104,   339,    55,   378,    49,     5,     6,   112,   267,   296,
       7,   227,   113,   238,   101,   120,   440,   180,    51,   258,
     183,   184,   294,   343,   344,   345,   346,   347,   348,     5,
       6,   261,   123,   185,   107,   230,     3,   232,   233,   214,
     378,   214,   181,   136,   367,   365,    92,   366,   422,   237,
     267,   188,   214,   423,   407,     1,     2,  -200,   374,   310,
       1,     2,   121,   231,     1,     2,  -144,   335,   269,   234,
     235,  -217,  -217,  -217,  -217,   331,     5,     6,   298,   262,
     114,   270,   265,    77,   277,   318,     5,     6,   142,     1,
       2,   351,   351,   351,   351,   351,   351,   351,   294,  -144,
     434,   394,   260,   143,   223,   144,    99,   143,   294,   144,
     286,   288,    92,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,   395,    80,    81,    82,    83,    84,    94,   335,   396,
    -217,  -217,  -217,   338,   292,     1,     2,   198,   152,   329,
     330,   377,     5,     6,   262,   301,   351,   301,   379,   413,
     414,   397,    92,   108,   284,   285,   301,   301,   384,   308,
       1,     2,   398,   431,   323,   324,    94,    94,   301,   411,
     320,   120,    58,   125,   291,    94,   376,   158,   143,   395,
     144,    94,    94,   294,    94,    94,   114,   396,   259,   227,
     135,   401,     5,     6,   138,   177,   152,   152,   369,   124,
     301,   371,   403,   301,   152,   373,   108,   386,    94,   255,
     256,   390,   391,   190,   284,   285,   177,   177,   410,   351,
     351,   284,   285,   301,   415,   301,   416,   301,   408,   149,
     424,   324,   400,   152,     3,   158,   158,   426,   421,   114,
       1,     2,  -201,   158,    90,     5,     6,     3,   336,   119,
     149,   149,   264,   402,   222,     3,   161,   425,   228,   229,
    -145,   392,    94,    94,   137,  -218,  -218,  -218,  -218,   227,
     266,   433,   158,   429,     5,     6,   239,   161,   161,   251,
     189,   161,   357,   404,    91,   301,    95,     5,     6,   253,
     375,   254,     0,   227,   412,     5,     6,    92,   108,   430,
     435,   354,   354,   354,   354,   354,   354,   354,   420,    94,
    -144,    94,    94,   234,   235,   439,    94,  -216,  -216,  -216,
    -216,     3,   108,   385,   239,   198,     1,     2,   192,   432,
       0,   301,     0,   324,  -218,  -218,  -218,     5,     6,     0,
     313,    94,     0,     0,    94,   358,   359,   360,   361,   362,
     363,   364,     0,   326,     0,     0,   295,   297,   309,     0,
       0,     5,     6,    58,   114,    94,   354,    94,   266,     0,
       5,     6,   356,   356,   356,   356,   356,   356,   356,   319,
       0,   321,   322,     0,     0,   311,  -216,  -216,  -216,     0,
     350,   350,   350,   350,   350,   350,   350,     0,     0,     0,
       0,     0,     0,   222,     0,     0,     0,     0,    94,    94,
     399,     0,   393,   177,   108,     0,   251,   352,   352,   352,
     352,   352,   352,   352,     0,     0,   239,     0,     0,     0,
     161,     0,   278,  -199,   279,   280,     0,   356,   325,   354,
     354,     0,     0,     0,   130,   131,     0,   149,   337,   340,
     341,     0,     0,     5,     6,   350,     0,   353,   353,   353,
     353,   353,   353,   353,     0,    94,     0,   239,     0,    94,
      94,   177,   177,     0,   161,   271,   272,   273,   274,     0,
       0,    94,   352,   427,   428,   355,   355,   355,   355,   355,
     355,   355,   275,   276,     0,     0,     1,     2,   139,   140,
     141,   281,   282,   283,     0,   149,   149,     0,     0,    94,
     356,   356,     0,   149,     0,     0,     0,     0,    94,   251,
       0,   326,   353,     0,   142,     0,     0,     0,   350,   350,
       0,     0,   161,   161,   196,   197,   198,     1,     2,   143,
     161,   144,   149,     5,     6,   145,   146,   299,     7,     0,
     355,     0,     0,     0,     0,   352,   352,     0,     0,     0,
       0,   199,     0,   200,   201,     0,   300,   147,   148,   161,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
     143,     0,   144,     0,     0,   196,   197,   198,     1,     2,
    -213,  -213,  -213,  -213,     0,   353,   353,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   325,   196,   197,   198,
       1,     2,   199,     0,   200,   201,     0,   300,   315,     0,
       0,     0,     0,   355,   355,    58,   203,   204,   205,     0,
       0,   143,     0,   144,   199,     0,   200,   201,     0,   300,
     370,   196,   197,   198,     1,     2,     0,    58,  -214,  -214,
    -214,  -214,     0,   143,     0,   144,     0,     0,     0,  -213,
    -213,  -213,     0,   196,   197,   198,     1,     2,   199,     0,
     200,   201,     0,   300,   372,     0,     0,   203,   204,   205,
       0,    58,     0,     0,     0,     0,     0,   143,     0,   144,
     199,     0,   200,   201,     0,   300,   380,     0,     0,   203,
     204,   205,     0,    58,     0,     0,     0,     0,     0,   143,
       0,   144,   196,   197,   198,     1,     2,  -214,  -214,  -214,
       0,   196,   197,   198,     1,     2,  -215,  -215,  -215,  -215,
       0,     0,     0,   203,   204,   205,     0,     0,     0,   199,
       0,   200,   201,     0,   300,   382,     0,     0,   199,     0,
     200,   201,    58,   300,   383,   203,   204,   205,   143,     0,
     144,    58,   196,   197,   198,     1,     2,   143,     0,   144,
       0,   196,   197,   198,     1,     2,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   199,
       0,   200,   201,     0,   202,  -215,  -215,  -215,   199,     0,
     200,   201,    58,   300,   203,   204,   205,     0,   143,     0,
     144,    58,     0,   203,   204,   205,     0,   143,     0,   144,
     196,   197,   198,     1,     2,   278,  -199,   279,   280,     0,
     196,   197,   198,     1,     2,   278,     0,   279,   280,   278,
       0,   279,   280,     0,     0,     0,     0,   199,     0,   200,
     201,   337,   342,     0,   203,   204,   205,   349,     0,     0,
      58,     0,   342,   203,   204,   205,   143,     0,   144,     0,
      58,     0,     0,     0,     0,     0,   143,     0,   144,     1,
       2,   278,     0,   279,   280,     0,     3,     0,     0,     0,
       0,     0,     0,     0,   281,   282,   283,   436,     0,   278,
       0,   279,   280,     0,   281,   282,   283,     0,   281,   282,
     283,     0,   203,   204,   205,   437,     0,     0,     0,     4,
       0,     0,   203,   204,   205,     0,     5,     6,     0,     0,
       0,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
     281,   282,   283,     0,    26,     1,     2,    27,    28,    29,
      30,    31,    92,  -221,  -221,     0,     0,     0,   281,   282,
     283,     0,     0,     0,     0,     0,   271,   272,   273,   274,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   275,   276,     4,     0,     0,     0,     0,
       0,     0,     5,     6,     0,     0,     0,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,     0,     0,     0,     0,
      26,  -221,     0,    27,    28,    29,    30,    31,     1,     2,
     139,   140,   141,     0,     0,     0,   108,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   142,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   114,
       0,   143,     0,   144,     0,     5,     6,   145,   146,     0,
       7,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   147,
     148
};

static const yytype_int16 yycheck[] =
{
       0,   139,   140,    96,   142,   141,    98,   139,   140,   106,
     142,    96,   105,   106,   137,   138,    17,   330,   111,     7,
     202,    35,   115,   116,   238,     0,   240,     0,     6,     7,
     115,   116,    21,    33,    48,   249,    37,    38,    39,    40,
     116,     0,   400,    43,   137,    11,    12,   261,    15,    50,
       6,     7,   137,    26,    54,    29,    56,    31,    33,    48,
      33,   137,   138,    43,   202,   141,    80,    35,     6,     7,
     202,   429,    45,    48,    33,    48,    49,    77,    78,   116,
      48,    48,    11,    12,   109,    44,    11,    12,     0,    48,
      33,    92,    93,    94,    82,    35,    52,    11,    12,    13,
     137,   138,   316,    81,   129,    48,     7,    36,    48,   109,
      31,    36,    87,    79,    87,    53,    54,    80,   300,    35,
      58,    48,    95,    15,    80,    98,   439,    35,    87,    80,
     130,   131,    48,   271,   272,   273,   274,   275,   276,    53,
      54,    17,   101,    81,    87,   145,    13,   147,   148,   287,
      79,   289,   125,   112,   290,   287,    13,   289,    31,   159,
     342,   134,   300,    36,   378,     6,     7,    21,   300,   245,
       6,     7,   264,   146,     6,     7,    15,   262,    24,    18,
      19,    20,    21,    22,    23,   261,    53,    54,    57,   189,
      47,    24,   192,    34,    21,    35,    53,    54,    34,     6,
       7,   277,   278,   279,   280,   281,   282,   283,    48,    15,
     424,   334,   185,    49,   290,    51,   189,    49,    48,    51,
     220,   221,    13,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,   334,    27,    28,    29,    30,    31,    48,   333,   334,
      89,    90,    91,    36,   227,     6,     7,     5,   334,   259,
     260,    16,    53,    54,   264,   238,   342,   240,    48,   392,
     393,    25,    13,    14,    11,    12,   249,   250,    36,    30,
       6,     7,    25,   421,   257,   258,    87,    88,   261,   386,
      35,   264,    43,   386,    31,    96,    35,   334,    49,   392,
      51,   102,   103,    48,   105,   106,    47,   392,    34,    48,
     111,    31,    53,    54,   115,   116,   392,   393,   291,   104,
     293,   294,    36,   296,   400,   298,    14,   327,   129,    44,
      45,   331,   332,   134,    11,    12,   137,   138,    48,   415,
     416,    11,    12,   316,    35,   318,    35,   320,    83,   116,
      80,   324,    29,   429,    13,   392,   393,    80,    80,    47,
       6,     7,    21,   400,    45,    53,    54,    13,   264,    98,
     137,   138,   189,   373,   141,    13,   116,   410,   143,   144,
      15,   333,   183,   184,   115,    20,    21,    22,    23,    48,
     202,   422,   429,    31,    53,    54,   161,   137,   138,   164,
     134,   141,   277,   376,    45,   378,    52,    53,    54,   174,
     300,   176,    -1,    48,   387,    53,    54,    13,    14,   419,
     426,   277,   278,   279,   280,   281,   282,   283,   401,   230,
      15,   232,   233,    18,    19,   435,   237,    20,    21,    22,
      23,    13,    14,   324,   209,     5,     6,     7,   134,   422,
      -1,   424,    -1,   426,    89,    90,    91,    53,    54,    -1,
     245,   262,    -1,    -1,   265,   277,   278,   279,   280,   281,
     282,   283,    -1,   258,    -1,    -1,   228,   229,   245,    -1,
      -1,    53,    54,    43,    47,   286,   342,   288,   300,    -1,
      53,    54,   277,   278,   279,   280,   281,   282,   283,   251,
      -1,   253,   254,    -1,    -1,   245,    89,    90,    91,    -1,
     277,   278,   279,   280,   281,   282,   283,    -1,    -1,    -1,
      -1,    -1,    -1,   290,    -1,    -1,    -1,    -1,   329,   330,
     342,    -1,   333,   334,    14,    -1,   301,   277,   278,   279,
     280,   281,   282,   283,    -1,    -1,   311,    -1,    -1,    -1,
     290,    -1,    20,    21,    22,    23,    -1,   342,   258,   415,
     416,    -1,    -1,    -1,    44,    45,    -1,   334,    36,   269,
     270,    -1,    -1,    53,    54,   342,    -1,   277,   278,   279,
     280,   281,   282,   283,    -1,   386,    -1,   352,    -1,   390,
     391,   392,   393,    -1,   334,    24,    25,    26,    27,    -1,
      -1,   402,   342,   415,   416,   277,   278,   279,   280,   281,
     282,   283,    41,    42,    -1,    -1,     6,     7,     8,     9,
      10,    89,    90,    91,    -1,   392,   393,    -1,    -1,   430,
     415,   416,    -1,   400,    -1,    -1,    -1,    -1,   439,   404,
      -1,   426,   342,    -1,    34,    -1,    -1,    -1,   415,   416,
      -1,    -1,   392,   393,     3,     4,     5,     6,     7,    49,
     400,    51,   429,    53,    54,    55,    56,    16,    58,    -1,
     342,    -1,    -1,    -1,    -1,   415,   416,    -1,    -1,    -1,
      -1,    30,    -1,    32,    33,    -1,    35,    77,    78,   429,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    51,    -1,    -1,     3,     4,     5,     6,     7,
      20,    21,    22,    23,    -1,   415,   416,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   426,     3,     4,     5,
       6,     7,    30,    -1,    32,    33,    -1,    35,    36,    -1,
      -1,    -1,    -1,   415,   416,    43,    95,    96,    97,    -1,
      -1,    49,    -1,    51,    30,    -1,    32,    33,    -1,    35,
      36,     3,     4,     5,     6,     7,    -1,    43,    20,    21,
      22,    23,    -1,    49,    -1,    51,    -1,    -1,    -1,    89,
      90,    91,    -1,     3,     4,     5,     6,     7,    30,    -1,
      32,    33,    -1,    35,    36,    -1,    -1,    95,    96,    97,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,
      30,    -1,    32,    33,    -1,    35,    36,    -1,    -1,    95,
      96,    97,    -1,    43,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    51,     3,     4,     5,     6,     7,    89,    90,    91,
      -1,     3,     4,     5,     6,     7,    20,    21,    22,    23,
      -1,    -1,    -1,    95,    96,    97,    -1,    -1,    -1,    30,
      -1,    32,    33,    -1,    35,    36,    -1,    -1,    30,    -1,
      32,    33,    43,    35,    36,    95,    96,    97,    49,    -1,
      51,    43,     3,     4,     5,     6,     7,    49,    -1,    51,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    32,    33,    -1,    35,    89,    90,    91,    30,    -1,
      32,    33,    43,    35,    95,    96,    97,    -1,    49,    -1,
      51,    43,    -1,    95,    96,    97,    -1,    49,    -1,    51,
       3,     4,     5,     6,     7,    20,    21,    22,    23,    -1,
       3,     4,     5,     6,     7,    20,    -1,    22,    23,    20,
      -1,    22,    23,    -1,    -1,    -1,    -1,    30,    -1,    32,
      33,    36,    35,    -1,    95,    96,    97,    30,    -1,    -1,
      43,    -1,    35,    95,    96,    97,    49,    -1,    51,    -1,
      43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,     6,
       7,    20,    -1,    22,    23,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    91,    36,    -1,    20,
      -1,    22,    23,    -1,    89,    90,    91,    -1,    89,    90,
      91,    -1,    95,    96,    97,    36,    -1,    -1,    -1,    46,
      -1,    -1,    95,    96,    97,    -1,    53,    54,    -1,    -1,
      -1,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      89,    90,    91,    -1,    81,     6,     7,    84,    85,    86,
      87,    88,    13,    11,    12,    -1,    -1,    -1,    89,    90,
      91,    -1,    -1,    -1,    -1,    -1,    24,    25,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    42,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    54,    -1,    -1,    -1,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,
      81,    79,    -1,    84,    85,    86,    87,    88,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    49,    -1,    51,    -1,    53,    54,    55,    56,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     6,     7,    13,    46,    53,    54,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    81,    84,    85,    86,
      87,    88,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   115,   116,   117,   121,   151,
     152,   153,   188,   189,   190,   193,   194,   197,    43,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,    34,   151,   153,
     166,   166,   166,   166,   166,     0,   101,   121,   121,   153,
     117,   197,    13,   101,   188,    52,   121,   124,   125,   151,
     187,    80,   121,   121,    31,   121,   121,   101,    14,   114,
     118,   154,    80,   151,    47,   128,   142,   154,   155,   124,
     151,   187,    82,   153,   166,   154,   198,   198,   118,   121,
      44,    45,   113,   119,   154,   188,   153,   142,   188,     8,
       9,    10,    34,    49,    51,    55,    56,    77,    78,   126,
     129,   130,   131,   132,   133,   134,   135,   136,   143,   144,
     146,   147,   148,   150,   151,   154,   155,   173,   175,   176,
     177,   178,   179,   183,   184,   185,   186,   188,   191,   193,
      35,   151,   118,   121,   121,    81,   120,   122,   151,   152,
     188,   192,   194,   195,   129,   129,     3,     4,     5,    30,
      32,    33,    35,    95,    96,    97,   126,   131,   143,   147,
     156,   157,   158,   159,   161,   166,   167,   168,   169,   170,
     171,   171,   126,   131,   148,   182,   171,    48,   137,   137,
     121,   151,   121,   121,    18,    19,   137,   121,    15,   137,
      17,    37,    38,    39,    40,    50,    92,    93,    94,   149,
      35,   137,   138,   137,   137,    44,    45,   112,    80,    34,
     151,    17,   121,   123,   125,   121,   168,   169,   171,    24,
      24,    24,    25,    26,    27,    41,    42,    21,    20,    22,
      23,    89,    90,    91,    11,    12,   121,   172,   121,    29,
      31,    31,   151,    35,    48,   138,    35,   138,    57,    16,
      35,   151,   161,   162,   163,   171,   174,   162,    30,   126,
     131,   147,   160,   166,   162,    36,   140,   162,    35,   138,
      35,   138,   138,   151,   151,   156,   166,   199,   200,   121,
     121,   131,   162,   127,   141,   155,   123,    36,    36,    36,
     156,   156,    35,   161,   161,   161,   161,   161,   161,    30,
     126,   131,   147,   156,   158,   159,   166,   167,   168,   168,
     168,   168,   168,   168,   168,   171,   171,   148,   145,   151,
      36,   151,    36,   151,   171,   174,    35,    16,    79,    48,
      36,   162,    36,    36,    36,   200,   121,   155,   196,   196,
     121,   121,   141,   188,   129,   154,   155,    25,    25,   168,
      29,    31,   121,    36,   151,   164,   165,   162,    83,   139,
      48,   198,   151,   129,   129,    35,    35,   144,   180,   181,
     151,    80,    31,    36,    80,   139,    80,   168,   168,    31,
     121,   161,   151,   165,   162,   199,    36,    36,   144,   121,
     196
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    98,    99,   100,   100,   100,   100,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   102,   102,   102,
     102,   102,   103,   103,   103,   103,   103,   104,   104,   104,
     104,   105,   105,   105,   105,   105,   106,   106,   106,   107,
     108,   109,   110,   110,   110,   110,   110,   111,   112,   112,
     113,   113,   114,   114,   114,   115,   115,   116,   116,   117,
     117,   118,   119,   119,   119,   119,   119,   119,   119,   120,
     120,   121,   121,   121,   121,   122,   122,   123,   124,   125,
     125,   125,   125,   126,   126,   126,   126,   126,   127,   127,
     127,   128,   128,   128,   129,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   131,   131,   131,   131,   131,
     132,   132,   133,   133,   134,   134,   135,   135,   136,   136,
     137,   137,   138,   138,   138,   139,   140,   140,   141,   141,
     141,   142,   142,   142,   142,   143,   143,   144,   144,   144,
     144,   144,   145,   145,   146,   146,   147,   147,   148,   148,
     148,   149,   149,   149,   149,   149,   149,   149,   150,   150,
     151,   151,   152,   153,   154,   155,   156,   157,   158,   158,
     158,   158,   158,   158,   159,   160,   160,   160,   160,   160,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     162,   162,   162,   162,   163,   164,   164,   165,   166,   167,
     167,   167,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   169,   169,   169,   169,   169,   169,   170,
     170,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     172,   172,   173,   174,   174,   175,   176,   177,   178,   179,
     180,   181,   181,   182,   182,   183,   184,   184,   185,   185,
     186,   186,   187,   188,   188,   189,   190,   191,   192,   193,
     194,   195,   195,   196,   196,   197,   197,   198,   198,   199,
     199,   199,   200,   200
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     4,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     1,     3,     7,     2,     4,     1,     1,     1,
       2,     2,     1,     2,     3,     1,     2,     1,     2,     3,
       2,     2,     2,     2,     1,     1,     2,     2,     2,     4,
       4,     1,     1,     2,     2,     3,     2,     2,     2,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     2,     3,
       3,     2,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     4,     3,     4,     2,     3,     3,     4,
       2,     3,     3,     5,     4,     1,     1,     2,     1,     2,
       2,     1,     1,     2,     2,     2,     2,     1,     1,     2,
       4,     1,     1,     3,     1,     1,     4,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       7,     4,     7,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     1,     3,     3,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     3,     5,     3,     2,     2,     2,     3,     6,
       2,     1,     3,     1,     3,     2,     2,     1,     2,     1,
       2,     2,     2,     1,     2,     1,     1,     5,     2,     2,
       1,     4,     4,     5,     6,     4,     4,     5,     6,     1,
       1,     1,     1,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
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
  unsigned long int yylno = yyrline[yyrule];
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
                       &(yyvsp[(yyi + 1) - (yynrhs)])
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
            /* Fall through.  */
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

  return yystpcpy (yyres, yystr) - yyres;
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
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 25 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  if (yycount == 0)
  {
	  yyformat = strdup("syntax error");
  }
  else if (yycount == 1)
  {
	  yyformat = strdup("syntax error, unexpected %s");
  }
  else if (yycount > 1)
  {
	  char* tmp = (char*)malloc(1000);
	  tmp[0] = 0;
	  strcat(tmp, "syntax error, unexpected %s, expecting %s");	  
	  {
		int i;
		for (i = 0; i < yycount - 2; i++)
		{
			strcat(tmp, " or %s");
		}
	  }
	  yyformat = tmp;
  }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
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
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
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
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

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
| yyreduce -- Do a reduction.  |
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
#line 125 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); root = (yyval.nodeval); }
#line 1938 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 3:
#line 129 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 1944 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 4:
#line 131 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 1950 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 5:
#line 133 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 1956 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 6:
#line 135 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 1962 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 7:
#line 139 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 1968 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 8:
#line 141 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 1974 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 9:
#line 143 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 1980 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 10:
#line 145 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 1986 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 11:
#line 147 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 1992 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 12:
#line 149 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 1998 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 13:
#line 151 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2004 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 14:
#line 153 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2010 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 15:
#line 155 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2016 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 16:
#line 157 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2022 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 17:
#line 161 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_IMPORT_UNIX); }
#line 2028 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 18:
#line 163 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_IMPORT_WINDOWS); }
#line 2034 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 19:
#line 165 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, OP_IMPORT_WINDOWS); }
#line 2040 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 20:
#line 167 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_IMPORT_LINUX); }
#line 2046 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 21:
#line 169 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_IMPORT_MACOSX); }
#line 2052 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 22:
#line 173 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_INCLUDE); }
#line 2058 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 23:
#line 175 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_INCLUDE_LINUX); }
#line 2064 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 24:
#line 177 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_INCLUDE_WINDOWS); }
#line 2070 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 25:
#line 179 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_INCLUDE_MACOSX); }
#line 2076 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 26:
#line 181 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_INCLUDE_UNIX); }
#line 2082 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 27:
#line 185 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_IMPORT_UNIX_PATH); }
#line 2088 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 28:
#line 187 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_IMPORT_WINDOWS_PATH); }
#line 2094 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 29:
#line 189 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_IMPORT_LINUX_PATH); }
#line 2100 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 30:
#line 191 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_IMPORT_MACOSX_PATH); }
#line 2106 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 31:
#line 195 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_INCLUDE_WINDOWS_PATH); }
#line 2112 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 32:
#line 197 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_INCLUDE_UNIX_PATH); }
#line 2118 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 33:
#line 199 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_INCLUDE_LINUX_PATH); }
#line 2124 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 34:
#line 201 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_INCLUDE_MACOSX_PATH); }
#line 2130 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 35:
#line 203 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_INCLUDE_PATH); }
#line 2136 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 36:
#line 207 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_COMPILER_FLAGS_MACOSX); }
#line 2142 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 37:
#line 209 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_COMPILER_FLAGS_UNIX); }
#line 2148 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 38:
#line 211 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_COMPILER_FLAGS_LINUX); }
#line 2154 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 39:
#line 215 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_CPP_ENTRY_POINT); }
#line 2160 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 40:
#line 219 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_ADD_CPP); }
#line 2166 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 41:
#line 223 "yacc.y" /* yacc.c:1654  */
    {(yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, DECL_CLASS);}
#line 2172 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 42:
#line 227 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, DECL_CLASS_NAME); }
#line 2178 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 43:
#line 229 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, DECL_CLASS_NAME); }
#line 2184 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 44:
#line 231 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-6].nodeval), (yyvsp[-2].nodeval), (yyvsp[-1].nodeval), DECL_CLASS_NAME); }
#line 2190 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 45:
#line 233 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, DECL_CLASS_WITH_TYPE); }
#line 2196 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 46:
#line 235 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-3].nodeval), (yyvsp[-2].nodeval), (yyvsp[0].nodeval), DECL_CLASS_WITH_TYPE); }
#line 2202 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 47:
#line 239 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, CLASS_TYPE_SINGLETON_DECL); }
#line 2208 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 48:
#line 243 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, INHERITANCE_PRIVATE); }
#line 2214 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 49:
#line 245 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, INHERITANCE_PUBLIC); }
#line 2220 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 50:
#line 249 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, SECTION_PRIVATE_DECL); }
#line 2226 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 51:
#line 251 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, SECTION_PUBLIC_DECL); }
#line 2232 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 52:
#line 255 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2238 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 53:
#line 257 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, DECL_CLASS_CONTENTS); }
#line 2244 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 54:
#line 259 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[-1].nodeval), (yyvsp[0].nodeval), DECL_CLASS_CONTENTS); }
#line 2250 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 55:
#line 263 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2256 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 56:
#line 265 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 2262 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 57:
#line 269 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2268 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 58:
#line 271 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 2274 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 59:
#line 275 "yacc.y" /* yacc.c:1654  */
    {(yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[-1].nodeval), (yyvsp[0].nodeval), GLOBAL_FUNC_DECL);}
#line 2280 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 60:
#line 277 "yacc.y" /* yacc.c:1654  */
    {(yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, GLOBAL_FUNC_DECL);}
#line 2286 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 61:
#line 281 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 2292 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 62:
#line 285 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 2298 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 63:
#line 287 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 2304 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 64:
#line 289 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2310 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 65:
#line 291 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2316 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 66:
#line 293 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 2322 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 67:
#line 295 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 2328 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 68:
#line 297 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 2334 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 69:
#line 301 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-3].nodeval), (yyvsp[-1].nodeval), (yyvsp[0].nodeval), CLASS_PROPERTY_DECL); }
#line 2340 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 70:
#line 303 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-3].nodeval), (yyvsp[-1].nodeval), (yyvsp[0].nodeval), CLASS_PROPERTY_DECL); }
#line 2346 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 71:
#line 307 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, OP_NEWLINE); }
#line 2352 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 72:
#line 309 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2358 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 73:
#line 311 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, OP_NEWLINE); }
#line 2364 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 74:
#line 313 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2370 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 75:
#line 317 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[-1].nodeval), (yyvsp[0].nodeval), FUNC_DECL); }
#line 2376 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 76:
#line 319 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, FUNC_DECL); }
#line 2382 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 77:
#line 323 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 2388 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 78:
#line 327 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 2394 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 79:
#line 331 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, FUNC_PARAMS); }
#line 2400 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 80:
#line 333 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, FUNC_PARAMS); }
#line 2406 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 81:
#line 335 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, FUNC_PARAMS); }
#line 2412 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 82:
#line 337 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, FUNC_PARAMS); }
#line 2418 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 83:
#line 341 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2424 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 84:
#line 343 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2430 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 85:
#line 345 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2436 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 86:
#line 347 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2442 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 87:
#line 349 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2448 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 88:
#line 353 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 2454 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 89:
#line 355 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-2].nodeval), (yyvsp[-1].nodeval), (yyvsp[0].nodeval), ""); }
#line 2460 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 90:
#line 357 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-2].nodeval), (yyvsp[-1].nodeval), (yyvsp[0].nodeval), ""); }
#line 2466 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 91:
#line 361 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 2472 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 92:
#line 363 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-2].nodeval), (yyvsp[-1].nodeval), (yyvsp[0].nodeval), ""); }
#line 2478 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 93:
#line 365 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-2].nodeval), (yyvsp[-1].nodeval), (yyvsp[0].nodeval), ""); }
#line 2484 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 94:
#line 369 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[0].nodeval), NULL, NULL, ""); }
#line 2490 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 95:
#line 373 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2496 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 96:
#line 375 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2502 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 97:
#line 377 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2508 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 98:
#line 379 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2514 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 99:
#line 381 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2520 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 100:
#line 383 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2526 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 101:
#line 385 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2532 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 102:
#line 387 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 2538 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 103:
#line 389 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[0].nodeval), NULL, NULL, ""); }
#line 2544 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 104:
#line 391 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2550 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 105:
#line 395 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2556 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 106:
#line 397 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2562 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 107:
#line 399 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2568 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 108:
#line 401 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2574 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 109:
#line 403 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2580 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 110:
#line 407 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[-1].nodeval), (yyvsp[0].nodeval), OP_PROP_METHOD_CALL); }
#line 2586 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 111:
#line 409 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[-1].nodeval), (yyvsp[0].nodeval), OP_PROP_METHOD_CALL); }
#line 2592 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 112:
#line 413 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, (yyvsp[-1].nodeval), (yyvsp[0].nodeval), OP_METHOD_CALL); }
#line 2598 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 113:
#line 415 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, (yyvsp[-2].nodeval), NULL, OP_METHOD_CALL); }
#line 2604 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 114:
#line 419 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[-1].nodeval), (yyvsp[0].nodeval), OP_METHOD_CALL); }
#line 2610 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 115:
#line 421 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-3].nodeval), (yyvsp[-2].nodeval), NULL, OP_METHOD_CALL); }
#line 2616 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 116:
#line 425 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, OP_FUNCTION_CALL); }
#line 2622 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 117:
#line 427 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), NULL, NULL, OP_FUNCTION_CALL); }
#line 2628 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 118:
#line 431 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, (yyvsp[-1].nodeval), (yyvsp[0].nodeval), OP_BASE_METHOD_CALL); }
#line 2634 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 119:
#line 433 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, (yyvsp[-2].nodeval), NULL, OP_BASE_METHOD_CALL); }
#line 2640 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 120:
#line 437 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, PROP_IN_CALL); }
#line 2646 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 121:
#line 439 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, PROP_IN_CALL); }
#line 2652 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 122:
#line 443 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), NULL, NULL, OP_FUNCTION_ALL_CALL_PARAMS); }
#line 2658 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 123:
#line 445 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-3].nodeval), (yyvsp[0].nodeval), NULL, OP_FUNCTION_ALL_CALL_PARAMS); }
#line 2664 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 124:
#line 447 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, (yyvsp[0].nodeval), NULL, OP_FUNCTION_ALL_CALL_PARAMS); }
#line 2670 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 125:
#line 451 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, FUNC_RESULT_OBJECT); }
#line 2676 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 126:
#line 455 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_FUNCTION_CALL_PARAMS); }
#line 2682 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 127:
#line 457 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, OP_FUNCTION_CALL_PARAMS); }
#line 2688 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 128:
#line 461 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[0].nodeval), NULL, NULL, ""); }
#line 2694 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 129:
#line 463 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 2700 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 130:
#line 465 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 2706 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 131:
#line 469 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[0].nodeval), NULL, NULL, ""); }
#line 2712 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 132:
#line 471 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[0].nodeval), NULL, NULL, ""); }
#line 2718 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 133:
#line 473 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 2724 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 134:
#line 475 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 2730 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 135:
#line 479 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), NULL, NULL, OP_INC); }
#line 2736 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 136:
#line 481 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), NULL, NULL, OP_DEC); }
#line 2742 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 137:
#line 485 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2748 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 138:
#line 487 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2754 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 139:
#line 489 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_RETURN); }
#line 2760 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 140:
#line 491 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, OP_RETURN); }
#line 2766 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 141:
#line 493 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2772 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 142:
#line 497 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_RETURN_PROPS); }
#line 2778 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 143:
#line 499 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), (yyvsp[-2].nodeval), NULL, OP_RETURN_PROPS); }
#line 2784 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 144:
#line 503 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2790 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 145:
#line 505 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2796 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 146:
#line 509 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-3].nodeval), (yyvsp[-1].nodeval), NULL, ACCESS_ARRAY_ELEMENT); }
#line 2802 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 147:
#line 511 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), NULL, NULL, ACCESS_ARRAY_ELEMENT); }
#line 2808 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 148:
#line 515 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, OP_EQUAL); }
#line 2814 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 149:
#line 517 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[-1].nodeval), (yyvsp[0].nodeval), OP_CHANGE_VALUE); }
#line 2820 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 150:
#line 519 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, OP_CONCAT); }
#line 2826 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 151:
#line 523 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, OP_PLUS_EQUAL); }
#line 2832 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 152:
#line 525 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, OP_MINUS_EQUAL); }
#line 2838 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 153:
#line 527 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, OP_MULTIPLY_EQUAL); }
#line 2844 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 154:
#line 529 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, OP_DIVISION_EQUAL); }
#line 2850 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 155:
#line 531 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, OP_AND_EQUAL); }
#line 2856 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 156:
#line 533 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, OP_OR_EQUAL); }
#line 2862 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 157:
#line 535 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, OP_XOR_EQUAL); }
#line 2868 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 158:
#line 539 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2874 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 159:
#line 541 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2880 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 160:
#line 545 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2886 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 161:
#line 547 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2892 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 162:
#line 551 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode(NULL, NULL, NULL, yylval.strval); }
#line 2898 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 163:
#line 555 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode(NULL, NULL, NULL, yylval.strval); }
#line 2904 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 164:
#line 559 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, TAB); }
#line 2910 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 165:
#line 563 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, TAB3); }
#line 2916 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 166:
#line 567 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode(NULL, NULL, NULL, yylval.strval); }
#line 2922 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 167:
#line 571 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode(NULL, NULL, NULL, yylval.strval); }
#line 2928 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 168:
#line 575 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2934 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 169:
#line 577 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode(NULL, NULL, NULL, yylval.strval); }
#line 2940 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 170:
#line 579 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-4].nodeval), (yyvsp[-1].nodeval), NULL, OP_FLOAT_DECL); }
#line 2946 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 171:
#line 581 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), NULL, NULL, OP_FLOAT_DECL); }
#line 2952 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 172:
#line 583 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-4].nodeval), (yyvsp[-1].nodeval), NULL, OP_FIXED_DECL); }
#line 2958 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 173:
#line 585 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), NULL, NULL, OP_FIXED_DECL); }
#line 2964 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 174:
#line 589 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode(NULL, NULL, NULL, yylval.strval); }
#line 2970 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 175:
#line 593 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2976 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 176:
#line 595 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2982 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 177:
#line 597 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 2988 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 178:
#line 599 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode(NULL, NULL, NULL, yylval.strval); }
#line 2994 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 179:
#line 601 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3000 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 180:
#line 605 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3006 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 181:
#line 607 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3012 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 182:
#line 609 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3018 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 183:
#line 611 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3024 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 184:
#line 613 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3030 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 185:
#line 615 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3036 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 186:
#line 617 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3042 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 187:
#line 619 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3048 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 188:
#line 621 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode(NULL, NULL, NULL, yylval.strval); }
#line 3054 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 189:
#line 623 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3060 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 190:
#line 627 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval);}
#line 3066 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 191:
#line 629 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3072 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 192:
#line 631 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3078 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 193:
#line 633 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3084 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 194:
#line 637 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-3].nodeval), (yyvsp[-1].nodeval), NULL, OP_CONSTRUCTOR); }
#line 3090 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 195:
#line 641 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_CONSTRUCTOR_PARAMS); }
#line 3096 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 196:
#line 643 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, OP_CONSTRUCTOR_PARAMS); }
#line 3102 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 197:
#line 647 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, OP_CONSTRUCTOR_PARAMETER); }
#line 3108 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 198:
#line 651 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode(NULL, NULL, NULL, yylval.strval); }
#line 3114 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 199:
#line 655 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3120 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 200:
#line 657 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3126 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 201:
#line 659 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3132 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 202:
#line 663 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3138 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 203:
#line 665 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3144 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 204:
#line 667 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), NULL, NULL, OP_BRACKETS); }
#line 3150 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 205:
#line 669 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), NULL, NULL, OP_BRACKETS); }
#line 3156 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 206:
#line 671 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, OP_MINUS); }
#line 3162 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 207:
#line 673 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, OP_PLUS); }
#line 3168 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 208:
#line 675 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, OP_MULTIPLY); }
#line 3174 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 209:
#line 677 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, OP_DIVISION); }
#line 3180 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 210:
#line 679 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, OP_MATH_AND); }
#line 3186 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 211:
#line 681 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, OP_MATH_OR); }
#line 3192 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 212:
#line 683 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, OP_MATH_XOR); }
#line 3198 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 213:
#line 687 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3204 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 214:
#line 689 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3210 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 215:
#line 691 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3216 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 216:
#line 693 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode(NULL, NULL, NULL, yylval.strval); }
#line 3222 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 217:
#line 695 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3228 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 218:
#line 697 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3234 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 219:
#line 701 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, VALUE_TRUE); }
#line 3240 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 220:
#line 703 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, VALUE_FALSE); }
#line 3246 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 221:
#line 707 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval);}
#line 3252 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 222:
#line 709 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[-1].nodeval), (yyvsp[0].nodeval), CMP_COMPLEX); }
#line 3258 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 223:
#line 711 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, CMP_EQUAL); }
#line 3264 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 224:
#line 713 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, CMP_NOT_EQUAL); }
#line 3270 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 225:
#line 715 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, CMP_LESS); }
#line 3276 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 226:
#line 717 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, CMP_MORE); }
#line 3282 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 227:
#line 719 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, CMP_LESS_EQUAL); }
#line 3288 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 228:
#line 721 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, CMP_MORE_EQUAL); }
#line 3294 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 229:
#line 723 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[-1].nodeval); }
#line 3300 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 230:
#line 727 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, OP_OR); }
#line 3306 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 231:
#line 729 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, OP_AND); }
#line 3312 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 232:
#line 733 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, OP_IF); }
#line 3318 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 233:
#line 737 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-4].nodeval), (yyvsp[-2].nodeval), (yyvsp[0].nodeval), OP_SHORT_IF); }
#line 3324 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 234:
#line 739 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[-1].nodeval); }
#line 3330 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 235:
#line 743 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_ELSE); }
#line 3336 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 236:
#line 747 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_BREAK); }
#line 3342 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 237:
#line 751 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_CONTINUE); }
#line 3348 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 238:
#line 755 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, OP_WHILE); }
#line 3354 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 239:
#line 759 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-4].nodeval), (yyvsp[-2].nodeval), (yyvsp[0].nodeval), OP_FOR); }
#line 3360 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 240:
#line 763 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, ""); }
#line 3366 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 241:
#line 767 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_COMMA); }
#line 3372 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 242:
#line 769 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, OP_COMMA); }
#line 3378 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 243:
#line 773 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_COMMA); }
#line 3384 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 244:
#line 775 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, OP_COMMA); }
#line 3390 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 245:
#line 779 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, OP_FUNC_PROP); }
#line 3396 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 246:
#line 783 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_MY_PROP); }
#line 3402 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 247:
#line 785 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, OP_MY_PROP); }
#line 3408 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 248:
#line 789 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_BASE_PROP); }
#line 3414 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 249:
#line 791 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, NULL, NULL, OP_BASE_PROP); }
#line 3420 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 250:
#line 795 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, OP_PROP); }
#line 3426 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 251:
#line 797 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, OP_PROP); }
#line 3432 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 252:
#line 801 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OUT_PARAM); }
#line 3438 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 253:
#line 805 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_SINGLELINE_COMMENT); }
#line 3444 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 254:
#line 807 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, OP_MULTILINE_COMMENT); }
#line 3450 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 255:
#line 811 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode(NULL, NULL, NULL, yylval.strval); }
#line 3456 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 256:
#line 815 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode(NULL, NULL, NULL, yylval.strval); }
#line 3462 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 257:
#line 819 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-3].nodeval), (yyvsp[-1].nodeval), (yyvsp[0].nodeval), OP_FOREACH); }
#line 3468 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 258:
#line 823 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, OP_CPP_DECL); }
#line 3474 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 259:
#line 827 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, OP_CPP); }
#line 3480 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 260:
#line 831 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mknode(NULL, NULL, NULL, yylval.strval); }
#line 3486 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 261:
#line 835 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, OP_PARAM_ANNOTATION); }
#line 3492 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 262:
#line 837 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, (yyvsp[0].nodeval), NULL, OP_PARAM_ANNOTATION); }
#line 3498 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 263:
#line 841 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-3].nodeval), (yyvsp[-1].nodeval), NULL, OP_PARAM_ANNOTATION_PROP); }
#line 3504 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 264:
#line 843 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-4].nodeval), (yyvsp[-2].nodeval), (yyvsp[0].nodeval), OP_PARAM_ANNOTATION_PROP); }
#line 3510 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 265:
#line 847 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-2].nodeval), (yyvsp[0].nodeval), NULL, OP_PARAM_ANNOTATION); }
#line 3516 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 266:
#line 849 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode(NULL, (yyvsp[0].nodeval), NULL, OP_PARAM_ANNOTATION); }
#line 3522 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 267:
#line 853 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-3].nodeval), (yyvsp[-1].nodeval), NULL, OP_PARAM_ANNOTATION_PROP); }
#line 3528 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 268:
#line 855 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-4].nodeval), (yyvsp[-2].nodeval), (yyvsp[0].nodeval), OP_PARAM_ANNOTATION_PROP); }
#line 3534 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 269:
#line 859 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3540 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 270:
#line 861 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3546 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 271:
#line 863 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = (yyvsp[0].nodeval); }
#line 3552 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 272:
#line 867 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[0].nodeval), NULL, NULL, OP_PARAM_ANNOTATION_PROP_VALUE); }
#line 3558 "yacc.tab.c" /* yacc.c:1654  */
    break;

  case 273:
#line 869 "yacc.y" /* yacc.c:1654  */
    { (yyval.nodeval) = mkcmdnode((yyvsp[-1].nodeval), (yyvsp[0].nodeval), NULL, OP_PARAM_ANNOTATION_PROP_VALUE); }
#line 3564 "yacc.tab.c" /* yacc.c:1654  */
    break;


#line 3568 "yacc.tab.c" /* yacc.c:1654  */
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

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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
