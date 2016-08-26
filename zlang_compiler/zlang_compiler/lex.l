%{
#include <stdio.h>
#include "node.h"
#include "yacc.tab.h"

#ifdef WIN32
#define YY_NO_UNISTD_H
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#define strdup _strdup
#endif

int parseOk;
char* lexErrorText = NULL;
int lexErrorLine = 0;

#define PRINT_TOKEN(a)  /* a */
//#define PRINT_TOKEN(a) printf(a);

void setParseFile(FILE* _parseFile)
{
	yyin = _parseFile;
	parseOk = 1;
}

int isParsingSuccessed()
{
	return parseOk;
}

char* getLexErrorText()
{
	return lexErrorText;
}

int getLexErrorLine()
{
	return lexErrorLine;
}

%}

%option yylineno

%%
[-]*[0-9]+\.[0-9]+      		PRINT_TOKEN("FP_NUMBER\n"); yylval.strval = strdup(yytext); return FP_NUMBER;
~[-]*[0-9]+\.[0-9]+      		PRINT_TOKEN("ARB_FP_NUMBER\n"); yylval.strval = strdup(yytext); return FP_ARB_NUMBER;
0x[a-fA-F0-9]+				PRINT_TOKEN("hex\n"); yylval.strval = strdup(yytext); return HEX_NUMBER;
[-]*[0-9]+              		PRINT_TOKEN("number\n"); yylval.strval = strdup(yytext); return NUMBER;
me					PRINT_TOKEN("ME\n"); return ME;
\(out\)					PRINT_TOKEN("out\n"); return OUT_SPEC;
[\(]					PRINT_TOKEN("parens start\n"); return PARENS_START;
[\)]					PRINT_TOKEN("parens end\n"); return PARENS_END;
\"([^\\\"]|\\.)*\"			PRINT_TOKEN("string\n"); yylval.strval = strdup(yytext); return STRING;
base					PRINT_TOKEN("base\n"); return BASE;
break					PRINT_TOKEN("break\n"); return BREAK;
continue				PRINT_TOKEN("continue\n"); return CONTINUE;
else 					PRINT_TOKEN("else\n"); return ELSE;
while                   		PRINT_TOKEN("while\n"); return WHILE;
foreach					PRINT_TOKEN("foreach\n"); return FOREACH;
in					PRINT_TOKEN("foreach\n"); return IN_CONSTRUCT;
cppinherit				PRINT_TOKEN("cpp::inherit\n"); return CPP_INHERIT;
unix::import::path			PRINT_TOKEN("unix::import::path\n"); return UNIX_IMPORT_PATH;
windows::import::path			PRINT_TOKEN("windows::import::path\n"); return WINDOWS_IMPORT_PATH;
linux::import::path			PRINT_TOKEN("linux::import::path\n"); return LINUX_IMPORT_PATH;
macosx::import::path			PRINT_TOKEN("macosx::import::path\n"); return MACOSX_IMPORT_PATH;
unix::import				PRINT_TOKEN("unix::import\n"); return UNIX_IMPORT;
windows::import				PRINT_TOKEN("windows::import\n"); return WINDOWS_IMPORT;
linux::import				PRINT_TOKEN("linux::import\n"); return LINUX_IMPORT;
macosx::import				PRINT_TOKEN("macosx::import\n"); return MACOSX_IMPORT;
include					PRINT_TOKEN("include\n"); return INCLUDE;
windows::include::path			PRINT_TOKEN("windows::include::path\n"); return WINDOWS_INCLUDE_PATH;
unix::include::path			PRINT_TOKEN("unix::include::path\n"); return UNIX_INCLUDE_PATH;
linux::include::path			PRINT_TOKEN("linux::include::path\n"); return LINUX_INCLUDE_PATH;
macosx::include::path			PRINT_TOKEN("macosx::include::path\n"); return MACOSX_INCLUDE_PATH;
include::path				PRINT_TOKEN("include::path\n"); return INCLUDE_PATH;
linux::include				PRINT_TOKEN("linux::include\n"); return LINUX_INCLUDE;
windows::include			PRINT_TOKEN("windows::include\n"); return WINDOWS_INCLUDE;
macosx::include				PRINT_TOKEN("macosx::include\n"); return MACOSX_INCLUDE;
unix::include				PRINT_TOKEN("unix::include\n"); return UNIX_INCLUDE;
unix::compiler::flags			PRINT_TOKEN("unix::compiler::flags\n"); return UNIX_COMPILER_FLAGS;
linux::compiler::flags			PRINT_TOKEN("linux::compiler::flags\n"); return LINUX_COMPILER_FLAGS;
macosx::compiler::flags			PRINT_TOKEN("macosx::compiler::flags\n"); return MACOSX_COMPILER_FLAGS;
cpp::entry::point			PRINT_TOKEN("cpp::entry::point\n"); return CPP_ENTRY_POINT;
Float					PRINT_TOKEN("FLOAT\n"); return FLOAT_DECL;
Fixed					PRINT_TOKEN("FLOAT\n"); return FIXED_DECL;
add					PRINT_TOKEN("add\n"); return ADD_CPP;
if                  			PRINT_TOKEN("if\n"); return IF;
true					PRINT_TOKEN("true\n"); return VAL_TRUE;
false					PRINT_TOKEN("false\n"); return VAL_FALSE;
return					PRINT_TOKEN("return\n"); return OPERATOR_RETURN;
[^f]or                 			PRINT_TOKEN("or\n"); return OR;
&=					PRINT_TOKEN("&=\n"); return OPERATOR_AND_EQUAL;
\|=					PRINT_TOKEN("|=\n"); return OPERATOR_OR_EQUAL;
[\^]=					PRINT_TOKEN("^=\n"); return OPERATOR_XOR_EQUAL;
&					PRINT_TOKEN("&\n"); return OPERATOR_MATH_AND;
\|					PRINT_TOKEN("|\n"); return OPERATOR_MATH_OR;
[\^]					PRINT_TOKEN("^\n"); return OPERATOR_MATH_XOR;
and                 			PRINT_TOKEN("and\n"); return AND;
for                  			PRINT_TOKEN("for\n"); return FOR;
private					PRINT_TOKEN("private\n"); return PRIVATE;
public					PRINT_TOKEN("public\n"); return PUBLIC;
singleton				PRINT_TOKEN("singleton\n"); return SINGLETON;
object					PRINT_TOKEN("object"); return OBJECT;
[a-z][a-zA-Z0-9_]*   			PRINT_TOKEN("id\n"); yylval.strval = strdup(yytext); return ID_FUNC;
[A-Z][a-zA-Z0-9_]*   			PRINT_TOKEN("id\n"); yylval.strval = strdup(yytext); return ID_CLASS;
\?					PRINT_TOKEN("?\n"); return QUESTION_SIGN;
:					PRINT_TOKEN(":\n"); return COLON;
\n                      		PRINT_TOKEN("newline\n"); return NEWLINE;
\t\t\t                 			PRINT_TOKEN("cd3\n"); return CODE_LEVEL3;
\t                  			PRINT_TOKEN("cd\n"); return CODE_LEVEL;
\[					PRINT_TOKEN("[\n"); return SQUARE_BRACKET_START;
\]					PRINT_TOKEN("]\n"); return SQUARE_BRACKET_END;
\.=					PRINT_TOKEN("concat\n"); return OPERATOR_CONCAT;
=[^=]					PRINT_TOKEN("equal\n"); return OPERATOR_EQUAL;
\+\+					PRINT_TOKEN("inc\n"); return OPERATOR_INCREMENT;
--					PRINT_TOKEN("dec\n"); return OPERATOR_DECREMENT;
-[^-=]					PRINT_TOKEN("minus\n"); return OPERATOR_MINUS;
\+[^\+=]				PRINT_TOKEN("plus\n"); return OPERATOR_PLUS;
[\*][^=]				PRINT_TOKEN("*\n"); return OPERATOR_MULTIPLY;
[/][^=]					PRINT_TOKEN("divide\n"); return OPERATOR_DIVISION;
-=					PRINT_TOKEN("-=\n"); return OPERATOR_MINUS_EQUAL;
\+=					PRINT_TOKEN("+=\n"); return OPERATOR_PLUS_EQUAL;
[\*]=					PRINT_TOKEN("*=\n"); return OPERATOR_MULTIPLY_EQUAL;
[/]=					PRINT_TOKEN("divide equal\n"); return OPERATOR_DIVISION_EQUAL;
\<=					PRINT_TOKEN("cmp less eq\n"); return COMPARE_LESS_EQUAL;
\>=					PRINT_TOKEN("cmp more eq\n"); return COMPARE_MORE_EQUAL;
\<					PRINT_TOKEN("cmp less\n"); return COMPARE_LESS;
\>					PRINT_TOKEN("cmp more\n"); return COMPARE_MORE;
==					PRINT_TOKEN("cmp eq\n"); return COMPARE_EQUAL;
!=					PRINT_TOKEN("cmp not eq\n"); return COMPARE_NOT_EQUAL;
\|\|					PRINT_TOKEN("cond or\n"); return CONDITION_OR;
;					PRINT_TOKEN("end part\n"); return END_PART;
'([^']|'')'				PRINT_TOKEN("symbol\n"); yylval.strval = strdup(yytext); return SYMBOL;
,					PRINT_TOKEN("comma\n"); return COMMA;
\.					PRINT_TOKEN("dot\n"); return DOT;
cpp@([^@]*)@cpp   			PRINT_TOKEN("cpp\n"); yylval.strval = strdup(yytext); return CPP;
@					PRINT_TOKEN("@"); return DOG;
[/][/][^\n]+[\n\r]+			PRINT_TOKEN("comment \n"); yylval.strval = strdup(yytext); return SINGLELINE_COMMENT;
[/][*][^*]*[*]+([^/*][^*]*[*]+)*[/]	PRINT_TOKEN("m comment\n"); yylval.strval = strdup(yytext); return MULTILINE_COMMENT;
[ ]					/**/
\r					/**/
.					/*printf("FATAL ERROR: unrecognized lexem: %s\n", yytext);*/lexErrorText = strdup(yytext); lexErrorLine = yylineno; parseOk = 0;
%%
