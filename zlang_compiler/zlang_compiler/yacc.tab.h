/* A Bison parser, made by GNU Bison 3.0.  */

/* Bison interface for Yacc-like parsers in C

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
#line 113 "yacc.y" /* yacc.c:1917  */

  int intval;
  char* strval;
  struct node* nodeval;  

#line 158 "yacc.tab.h" /* yacc.c:1917  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_YACC_TAB_H_INCLUDED  */
