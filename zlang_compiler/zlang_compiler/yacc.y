%{
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
%}

%define parse.error verbose

%token HEX_NUMBER FP_NUMBER NUMBER ID_FUNC ID_CLASS WHILE IF FOR OR AND NEWLINE CODE_LEVEL SQUARE_BRACKET_START SQUARE_BRACKET_END OPERATOR_EQUAL OPERATOR_INCREMENT OPERATOR_DECREMENT OPERATOR_MINUS OPERATOR_PLUS OPERATOR_MULTIPLY OPERATOR_DIVISION COMPARE_LESS COMPARE_MORE COMPARE_EQUAL COMPARE_NOT_EQUAL CONDITION_OR END_PART SYMBOL COMMA VAL_TRUE VAL_FALSE OPERATOR_RETURN PARENS_START PARENS_END OPERATOR_PLUS_EQUAL OPERATOR_MINUS_EQUAL OPERATOR_MULTIPLY_EQUAL OPERATOR_DIVISION_EQUAL COMPARE_LESS_EQUAL COMPARE_MORE_EQUAL STRING PRIVATE PUBLIC SINGLETON CODE_LEVEL3 DOT ME OPERATOR_CONCAT BASE OUT_SPEC MULTILINE_COMMENT SINGLELINE_COMMENT ELSE FOREACH IN_CONSTRUCT CPP UNIX_IMPORT WINDOWS_IMPORT LINUX_IMPORT MACOSX_IMPORT INCLUDE LINUX_INCLUDE WINDOWS_INCLUDE MACOSX_INCLUDE UNIX_INCLUDE UNIX_IMPORT_PATH WINDOWS_IMPORT_PATH LINUX_IMPORT_PATH MACOSX_IMPORT_PATH WINDOWS_INCLUDE_PATH UNIX_INCLUDE_PATH LINUX_INCLUDE_PATH MACOSX_INCLUDE_PATH ADD_CPP BREAK CONTINUE QUESTION_SIGN COLON DOG CPP_INHERIT OBJECT INCLUDE_PATH UNIX_COMPILER_FLAGS LINUX_COMPILER_FLAGS MACOSX_COMPILER_FLAGS CPP_ENTRY_POINT OPERATOR_MATH_AND OPERATOR_MATH_OR OPERATOR_MATH_XOR OPERATOR_AND_EQUAL OPERATOR_OR_EQUAL OPERATOR_XOR_EQUAL FP_ARB_NUMBER FLOAT_DECL FIXED_DECL

%union {
  int intval;
  char* strval;
  struct node* nodeval;  
};

%type<strval> ID_FUNC ID_CLASS SYMBOL CPP
%type<intval> NUMBER
%type<nodeval> ELSE WHILE IF FOR OR NEWLINE CODE_LEVEL SQUARE_BRACKET_START SQUARE_BRACKET_END OPERATOR_EQUAL OPERATOR_INCREMENT OPERATOR_DECREMENT OPERATOR_MINUS OPERATOR_PLUS COMPARE_LESS COMPARE_EQUAL COMPARE_NOT_EQUAL CONDITION_OR END_PART COMMA program commands command command_simple array_element making_equal_simple left_equal_part right_equal_part math_exp condition condition_operator if_operator while_operator for_operator comma_commands initializations operand id_exp number_exp function parameters cmd code_levels code_level_exp VAL_TRUE VAL_FALSE OPERATOR_RETURN operator_change_value STRING string_exp function_call function_call_parameters program_parts class section_type functions_and_properties function_or_property property class_type fps code_level3_exp function_commands comma_commands_for newlines fp_number_exp hex_number_exp my_prop_exp var string_right_part prop_exp class_decl BASE props_in_call out_var_exp function_call_parameters_with_parens singleline_comment_exp multiline_comment_exp MULTILINE_COMMENT SINGLELINE_COMMENT comment boolean_var else_operator right_equal_part_min FOREACH IN_CONSTRUCT foreach_operator array_element_var cpp_operator cpp_exp import_directive include_directive program_part import_path_directive include_path_directive add_cpp_directive math_unary_exp break_operator continue_operator one_line_if_operator function_parameter_annotation method_parameter_annotation function_annotation_properties method_annotation_properties annotation_values section_exp cpp_declaration annotation_value id_func_exp id_class_exp global_function global_code_levels global_function_commands global_func_commands return_props func_result_prop_exp object_exp prop_method_call internal_method_call var_method_call standard_function_call base_method_call global_function_impl function_parameters_annotation compiler_flags_directive entry_point_directive constructor constructor_params constructor_parameter base_prop_exp OPERATOR_MATH_AND OPERATOR_MATH_OR OPERATOR_MATH_XOR OPERATOR_AND_EQUAL OPERATOR_OR_EQUAL OPERATOR_XOR_EQUAL plus_exp FP_ARB_NUMBER FLOAT_DECL FIXED_DECL FP_NUMBER fp_simple_number_exp

%%

program: program_parts 				{ $$ = $1; root = $$; }
	;

program_parts: 
	program_part 				{ $$ = $1; }   		
	| 
	newlines program_part 			{ $$ = mknode($1, $2, NULL, ""); }   		
	| 
	program_parts program_part 		{ $$ = mknode($1, $2, NULL, ""); }
	|
	program_parts newlines program_part 	{ $$ = mknode($1, $3, NULL, ""); }
	;

program_part:
	class				{ $$ = $1; }
	|
	global_function			{ $$ = $1; }
	|
	import_directive		{ $$ = $1; }
	|
	include_directive		{ $$ = $1; }
	|
	import_path_directive		{ $$ = $1; }
	|
	include_path_directive		{ $$ = $1; }
	|
	compiler_flags_directive	{ $$ = $1; }
	|
	add_cpp_directive		{ $$ = $1; }
	|
	entry_point_directive		{ $$ = $1; }
	|
	cpp_operator			{ $$ = $1; }
	;

import_directive:
	UNIX_IMPORT string_exp 				{ $$ = mkcmdnode($2, NULL, NULL, OP_IMPORT_UNIX); }
	|
	WINDOWS_IMPORT string_exp 			{ $$ = mkcmdnode($2, NULL, NULL, OP_IMPORT_WINDOWS); }
	|
	WINDOWS_IMPORT string_exp COMMA string_exp	{ $$ = mkcmdnode($2, $4, NULL, OP_IMPORT_WINDOWS); }
	|
	LINUX_IMPORT string_exp 			{ $$ = mkcmdnode($2, NULL, NULL, OP_IMPORT_LINUX); }
	|
	MACOSX_IMPORT string_exp 			{ $$ = mkcmdnode($2, NULL, NULL, OP_IMPORT_MACOSX); }
	;

include_directive:
	INCLUDE string_exp		{ $$ = mkcmdnode($2, NULL, NULL, OP_INCLUDE); }
	|
	LINUX_INCLUDE string_exp 	{ $$ = mkcmdnode($2, NULL, NULL, OP_INCLUDE_LINUX); }
	|
	WINDOWS_INCLUDE string_exp 	{ $$ = mkcmdnode($2, NULL, NULL, OP_INCLUDE_WINDOWS); }
	|
	MACOSX_INCLUDE string_exp 	{ $$ = mkcmdnode($2, NULL, NULL, OP_INCLUDE_MACOSX); }
	|
	UNIX_INCLUDE string_exp 	{ $$ = mkcmdnode($2, NULL, NULL, OP_INCLUDE_UNIX); }
	;

import_path_directive:
	UNIX_IMPORT_PATH string_exp	{ $$ = mkcmdnode($2, NULL, NULL, OP_IMPORT_UNIX_PATH); }
	|
	WINDOWS_IMPORT_PATH string_exp 	{ $$ = mkcmdnode($2, NULL, NULL, OP_IMPORT_WINDOWS_PATH); }
	|
	LINUX_IMPORT_PATH string_exp	{ $$ = mkcmdnode($2, NULL, NULL, OP_IMPORT_LINUX_PATH); }
	|
	MACOSX_IMPORT_PATH string_exp 	{ $$ = mkcmdnode($2, NULL, NULL, OP_IMPORT_MACOSX_PATH); }
	;

include_path_directive:
	WINDOWS_INCLUDE_PATH string_exp	{ $$ = mkcmdnode($2, NULL, NULL, OP_INCLUDE_WINDOWS_PATH); }
	|
	UNIX_INCLUDE_PATH string_exp 	{ $$ = mkcmdnode($2, NULL, NULL, OP_INCLUDE_UNIX_PATH); }
	|
	LINUX_INCLUDE_PATH string_exp	{ $$ = mkcmdnode($2, NULL, NULL, OP_INCLUDE_LINUX_PATH); }
	|
	MACOSX_INCLUDE_PATH string_exp 	{ $$ = mkcmdnode($2, NULL, NULL, OP_INCLUDE_MACOSX_PATH); }
	|
	INCLUDE_PATH string_exp 	{ $$ = mkcmdnode($2, NULL, NULL, OP_INCLUDE_PATH); }
	;

compiler_flags_directive:
	MACOSX_COMPILER_FLAGS string_exp	{ $$ = mkcmdnode($2, NULL, NULL, OP_COMPILER_FLAGS_MACOSX); }
	|
	UNIX_COMPILER_FLAGS string_exp 		{ $$ = mkcmdnode($2, NULL, NULL, OP_COMPILER_FLAGS_UNIX); }
	|                               
	LINUX_COMPILER_FLAGS string_exp		{ $$ = mkcmdnode($2, NULL, NULL, OP_COMPILER_FLAGS_LINUX); }
	;

entry_point_directive:
	CPP_ENTRY_POINT string_exp		{ $$ = mkcmdnode($2, NULL, NULL, OP_CPP_ENTRY_POINT); }
	;

add_cpp_directive:
	ADD_CPP string_exp		{ $$ = mkcmdnode($2, NULL, NULL, OP_ADD_CPP); }
	;

class:
	class_decl newlines functions_and_properties {$$ = mkcmdnode($1, $3, NULL, DECL_CLASS);}
	;

class_decl:
	id_class_exp                          						{ $$ = mkcmdnode($1, NULL, NULL, DECL_CLASS_NAME); }
	|
	id_class_exp COLON id_class_exp							{ $$ = mkcmdnode($1, $3, NULL, DECL_CLASS_NAME); }
	|
	id_class_exp COLON CPP_INHERIT PARENS_START section_exp id_exp PARENS_END  	{ $$ = mkcmdnode($1, $5, $6, DECL_CLASS_NAME); }
	|
	class_type id_class_exp  							{ $$ = mkcmdnode($1, $2, NULL, DECL_CLASS_WITH_TYPE); }
	|
	class_type id_class_exp COLON id_class_exp	 				{ $$ = mkcmdnode($1, $2, $4, DECL_CLASS_WITH_TYPE); }
	;

class_type:
	SINGLETON		{ $$ = mkcmdnode(NULL, NULL, NULL, CLASS_TYPE_SINGLETON_DECL); }
	;

section_exp:
	PRIVATE			{ $$ = mkcmdnode(NULL, NULL, NULL, INHERITANCE_PRIVATE); } 
	|
	PUBLIC                  { $$ = mkcmdnode(NULL, NULL, NULL, INHERITANCE_PUBLIC); } 
	;

section_type:
	PRIVATE	newlines	{ $$ = mkcmdnode($2, NULL, NULL, SECTION_PRIVATE_DECL); } 
	|
	PUBLIC	newlines	{ $$ = mkcmdnode($2, NULL, NULL, SECTION_PUBLIC_DECL); } 
	;

functions_and_properties:
	function_or_property 					{ $$ = $1; }   		
	| 
	functions_and_properties function_or_property 		{ $$ = mkcmdnode($1, $2, NULL, DECL_CLASS_CONTENTS); }
	|
	functions_and_properties newlines function_or_property 	{ $$ = mkcmdnode($1, $2, $3, DECL_CLASS_CONTENTS); }
	;

function_parameters_annotation:
	function_parameter_annotation					{ $$ = $1; }
	|
	function_parameters_annotation function_parameter_annotation	{ $$ = mknode($1, $2, NULL, ""); }
	;

global_function:
	global_function_impl					{ $$ = $1; }
	|
	function_parameters_annotation global_function_impl	{ $$ = mknode($1, $2, NULL, ""); }
	;

global_function_impl:
	id_exp parameters global_function_commands		{$$ = mkcmdnode($1, $2, $3, GLOBAL_FUNC_DECL);}
	|
	id_exp global_function_commands				{$$ = mkcmdnode($1, $2, NULL, GLOBAL_FUNC_DECL);}
	;

function_or_property:
	code_level_exp fps					{ $$ = mknode($1, $2, NULL, ""); }
	;

fps:
	code_level_exp function 				{ $$ = mknode($1, $2, NULL, ""); }   		
	|
	code_level_exp property 				{ $$ = mknode($1, $2, NULL, ""); }   		
	|
	section_type						{ $$ = $1; }
	|
	comment							{ $$ = $1; }
	|
	code_level_exp comment					{ $$ = mknode($1, $2, NULL, ""); }
	|
	code_level_exp method_parameter_annotation		{ $$ = mknode($1, $2, NULL, ""); }	
	|
	code_level_exp cpp_declaration				{ $$ = mknode($1, $2, NULL, ""); }
	;

property:
	id_exp OPERATOR_EQUAL right_equal_part newlines		{ $$ = mkcmdnode($1, $3, $4, CLASS_PROPERTY_DECL); } 
	|
	id_exp OPERATOR_EQUAL function_call newlines		{ $$ = mkcmdnode($1, $3, $4, CLASS_PROPERTY_DECL); } 
	;

newlines: 
	NEWLINE					{ $$ = mkcmdnode(NULL, NULL, NULL, OP_NEWLINE); }
	|
	comment					{ $$ = $1; }
	|
	newlines NEWLINE                        { $$ = mkcmdnode(NULL, NULL, NULL, OP_NEWLINE); }
	|
	newlines comment                        { $$ = $2; }
	;

function:
	id_func_exp parameters function_commands	{ $$ = mkcmdnode($1, $2, $3, FUNC_DECL); }
	|
	id_func_exp function_commands			{ $$ = mkcmdnode($1, $2, NULL, FUNC_DECL); }
	;

function_commands:
	newlines commands 			{ $$ = mknode($1, $2, NULL, ""); }
	;

global_function_commands:
	newlines global_func_commands		{ $$ = mknode($1, $2, NULL, ""); }
	;

parameters:
	id_exp					{ $$ = mkcmdnode($1, NULL, NULL, FUNC_PARAMS); }   		
	|
	out_var_exp				{ $$ = mkcmdnode($1, NULL, NULL, FUNC_PARAMS); }   		
	|
	parameters id_exp			{ $$ = mkcmdnode($1, $2, NULL, FUNC_PARAMS); }
	|
	parameters out_var_exp			{ $$ = mkcmdnode($1, $2, NULL, FUNC_PARAMS); }
	;

var:
	id_exp					{ $$ = $1; }
	|
	my_prop_exp				{ $$ = $1; }
	|
	base_prop_exp				{ $$ = $1; }
	|
	prop_exp				{ $$ = $1; }
	|
	func_result_prop_exp			{ $$ = $1; }
	;

commands: 
	code_levels command 			{ $$ = mknode($1, $2, NULL, ""); }
	| 	
	commands code_levels command  		{ $$ = mknode($1, $2, $3, ""); }
	|
	commands comment command 		{ $$ = mknode($1, $2, $3, ""); }
        ;

global_func_commands: 
	global_code_levels command 			{ $$ = mknode($1, $2, NULL, ""); }
	| 	
	global_func_commands global_code_levels command	{ $$ = mknode($1, $2, $3, ""); }
	|
	global_func_commands comment command 		{ $$ = mknode($1, $2, $3, ""); }
        ;

command:
	cmd 					{ $$ = mknode($1, NULL, NULL, ""); }
	;

cmd:
	if_operator		{ $$ = $1; }
	|
	else_operator		{ $$ = $1; }
	|
	while_operator		{ $$ = $1; }
	|
	for_operator		{ $$ = $1; }
	|
	foreach_operator	{ $$ = $1; }
	|
	break_operator		{ $$ = $1; }
	|
	continue_operator	{ $$ = $1; }
	|
	command_simple newlines	{ $$ = mknode($1, $2, NULL, ""); }
	|
	comment 		{ $$ = mknode($1, NULL, NULL, ""); }
	|
	cpp_operator		{ $$ = $1; }
        ;      

function_call:
	prop_method_call	{ $$ = $1; }
	|
	internal_method_call	{ $$ = $1; }
	|                                               
	var_method_call		{ $$ = $1; }
	|
	standard_function_call	{ $$ = $1; }
	|
	base_method_call	{ $$ = $1; }
	;

prop_method_call:
	my_prop_exp props_in_call function_call_parameters_with_parens		{ $$ = mkcmdnode($1, $2, $3, OP_PROP_METHOD_CALL); }
	|
	prop_exp props_in_call function_call_parameters_with_parens		{ $$ = mkcmdnode($1, $2, $3, OP_PROP_METHOD_CALL); }
	;

internal_method_call:
	ME props_in_call function_call_parameters_with_parens			{ $$ = mkcmdnode(NULL, $2, $3, OP_METHOD_CALL); }
	|                                               
	ME props_in_call PARENS_START PARENS_END				{ $$ = mkcmdnode(NULL, $2, NULL, OP_METHOD_CALL); }
	;

var_method_call:
	id_exp props_in_call function_call_parameters_with_parens		{ $$ = mkcmdnode($1, $2, $3, OP_METHOD_CALL); }
	|
	id_exp props_in_call PARENS_START PARENS_END				{ $$ = mkcmdnode($1, $2, NULL, OP_METHOD_CALL); }
	;

standard_function_call:
	id_exp function_call_parameters_with_parens				{ $$ = mkcmdnode($1, $2, NULL, OP_FUNCTION_CALL); }
	|
	id_exp PARENS_START PARENS_END						{ $$ = mkcmdnode($1, NULL, NULL, OP_FUNCTION_CALL); }
	;

base_method_call:
	BASE props_in_call function_call_parameters_with_parens			{ $$ = mkcmdnode(NULL, $2, $3, OP_BASE_METHOD_CALL); }
	|
	BASE props_in_call PARENS_START PARENS_END				{ $$ = mkcmdnode(NULL, $2, NULL, OP_BASE_METHOD_CALL); }
	;

props_in_call:
	DOT id_exp					{ $$ = mkcmdnode($2, NULL, NULL, PROP_IN_CALL); }
	|
	props_in_call DOT id_exp                        { $$ = mkcmdnode($1, $3, NULL, PROP_IN_CALL); }
	;

function_call_parameters_with_parens:
	PARENS_START function_call_parameters PARENS_END 		{ $$ = mkcmdnode($2, NULL, NULL, OP_FUNCTION_ALL_CALL_PARAMS); }
	|
	PARENS_START function_call_parameters PARENS_END DOT object_exp	{ $$ = mkcmdnode($2, $5, NULL, OP_FUNCTION_ALL_CALL_PARAMS); }
	|
	PARENS_START PARENS_END DOT object_exp 				{ $$ = mkcmdnode(NULL, $4, NULL, OP_FUNCTION_ALL_CALL_PARAMS); }
	;

object_exp:
	OBJECT								{ $$ = mkcmdnode(NULL, NULL, NULL, FUNC_RESULT_OBJECT); }
	;

function_call_parameters:
	right_equal_part				{ $$ = mkcmdnode($1, NULL, NULL, OP_FUNCTION_CALL_PARAMS); }
	|
	function_call_parameters right_equal_part 	{ $$ = mkcmdnode($1, $2, NULL, OP_FUNCTION_CALL_PARAMS); }
	;

code_levels:
	code_level3_exp  				{ $$ = mknode($1, NULL, NULL, ""); }
	|
	code_levels code_level_exp 			{ $$ = mknode($1, $2, NULL, ""); }
	|
	code_levels code_level3_exp 			{ $$ = mknode($1, $2, NULL, ""); }
	;

global_code_levels:
	code_level_exp  				{ $$ = mknode($1, NULL, NULL, ""); }
	|
	code_level3_exp  				{ $$ = mknode($1, NULL, NULL, ""); }
	|
	global_code_levels code_level_exp 		{ $$ = mknode($1, $2, NULL, ""); }
	|
	global_code_levels code_level3_exp 		{ $$ = mknode($1, $2, NULL, ""); }
	;

math_unary_exp:
	var OPERATOR_INCREMENT				{ $$ = mkcmdnode($1, NULL, NULL, OP_INC); }
	|                               	  
	var OPERATOR_DECREMENT				{ $$ = mkcmdnode($1, NULL, NULL, OP_DEC); }
	;
	           
command_simple:
	math_unary_exp					{ $$ = $1; }
	|                                 
	making_equal_simple				{ $$ = $1; }
	|
	OPERATOR_RETURN condition			{ $$ = mkcmdnode($2, NULL, NULL, OP_RETURN); }
	|
	OPERATOR_RETURN condition COMMA return_props	{ $$ = mkcmdnode($2, $4, NULL, OP_RETURN); }
	|
	function_call					{ $$ = $1; }
	;

return_props:
	id_exp						{ $$ = mkcmdnode($1, NULL, NULL, OP_RETURN_PROPS); }
	|
	return_props COMMA id_exp			{ $$ = mkcmdnode($3, $1, NULL, OP_RETURN_PROPS); }
	;

array_element_var:
	var						{ $$ = $1; }
	|
	array_element					{ $$ = $1; }
	;

array_element:
	array_element_var SQUARE_BRACKET_START right_equal_part SQUARE_BRACKET_END 	{ $$ = mkcmdnode($1, $3, NULL, ACCESS_ARRAY_ELEMENT); }
	|
	array_element_var SQUARE_BRACKET_START SQUARE_BRACKET_END 			{ $$ = mkcmdnode($1, NULL, NULL, ACCESS_ARRAY_ELEMENT); }
	;

making_equal_simple:
	left_equal_part OPERATOR_EQUAL right_equal_part		{ $$ = mkcmdnode($1, $3, NULL, OP_EQUAL); }
	|
	left_equal_part operator_change_value right_equal_part 	{ $$ = mkcmdnode($1, $2, $3, OP_CHANGE_VALUE); }
	|
	left_equal_part OPERATOR_CONCAT string_right_part  	{ $$ = mkcmdnode($1, $3, NULL, OP_CONCAT); }
	;

operator_change_value:
	OPERATOR_PLUS_EQUAL		{ $$ = mkcmdnode(NULL, NULL, NULL, OP_PLUS_EQUAL); }
	|
	OPERATOR_MINUS_EQUAL		{ $$ = mkcmdnode(NULL, NULL, NULL, OP_MINUS_EQUAL); }
	|
	OPERATOR_MULTIPLY_EQUAL		{ $$ = mkcmdnode(NULL, NULL, NULL, OP_MULTIPLY_EQUAL); }
	|
	OPERATOR_DIVISION_EQUAL		{ $$ = mkcmdnode(NULL, NULL, NULL, OP_DIVISION_EQUAL); }
	|
	OPERATOR_AND_EQUAL		{ $$ = mkcmdnode(NULL, NULL, NULL, OP_AND_EQUAL); }
	|
	OPERATOR_OR_EQUAL		{ $$ = mkcmdnode(NULL, NULL, NULL, OP_OR_EQUAL); }
	|
	OPERATOR_XOR_EQUAL		{ $$ = mkcmdnode(NULL, NULL, NULL, OP_XOR_EQUAL); }	
	;

left_equal_part:         
	var		{ $$ = $1; }
	|
	array_element   { $$ = $1; }
	;

id_exp:
	id_func_exp	{ $$ = $1; }
	|
	id_class_exp	{ $$ = $1; }
	;

id_func_exp:
	ID_FUNC		{ $$ = mknode(NULL, NULL, NULL, yylval.strval); }
	;

id_class_exp:
	ID_CLASS	{ $$ = mknode(NULL, NULL, NULL, yylval.strval); }
	;

code_level_exp:
	CODE_LEVEL	{ $$ = mkcmdnode(NULL, NULL, NULL, TAB); }
	;

code_level3_exp:
	CODE_LEVEL3	{ $$ = mkcmdnode(NULL, NULL, NULL, TAB3); }
	;

number_exp:
	NUMBER		{ $$ = mknode(NULL, NULL, NULL, yylval.strval); }
	;

fp_simple_number_exp:
	FP_NUMBER									{ $$ = mknode(NULL, NULL, NULL, yylval.strval); }
	;

fp_number_exp:
	fp_simple_number_exp									{ $$ = $1; }
	|
	FP_ARB_NUMBER										{ $$ = mknode(NULL, NULL, NULL, yylval.strval); }
	|
	FLOAT_DECL COMPARE_LESS number_exp COMPARE_MORE PARENS_START math_exp PARENS_END	{ $$ = mkcmdnode($3, $6, NULL, OP_FLOAT_DECL); }	
	|
	FLOAT_DECL COMPARE_LESS number_exp COMPARE_MORE 					{ $$ = mkcmdnode($3, NULL, NULL, OP_FLOAT_DECL); }	
	|
	FIXED_DECL COMPARE_LESS number_exp COMPARE_MORE PARENS_START math_exp PARENS_END	{ $$ = mkcmdnode($3, $6, NULL, OP_FIXED_DECL); }	
	|
	FIXED_DECL COMPARE_LESS number_exp COMPARE_MORE						{ $$ = mkcmdnode($3, NULL, NULL, OP_FIXED_DECL); }	
	;

hex_number_exp:
	HEX_NUMBER	{ $$ = mknode(NULL, NULL, NULL, yylval.strval); }
	;

string_right_part:
	var		{ $$ = $1; }
	|
	array_element	{ $$ = $1; }
	|
	string_exp	{ $$ = $1; }
	|
	SYMBOL		{ $$ = mknode(NULL, NULL, NULL, yylval.strval); }
	|
	function_call	{ $$ = $1; }
	;

right_equal_part_min:
	math_exp	{ $$ = $1; }
	|
        number_exp	{ $$ = $1; }
	|
	function_call	{ $$ = $1; }
	|
        fp_number_exp	{ $$ = $1; }
	|
        hex_number_exp	{ $$ = $1; }
	|
	var		{ $$ = $1; }
	|
	array_element	{ $$ = $1; }
	|
	string_exp	{ $$ = $1; }
	|
	SYMBOL		{ $$ = mknode(NULL, NULL, NULL, yylval.strval); }
	|
	boolean_var	{ $$ = $1; }
	;

right_equal_part:
	one_line_if_operator 	{ $$ = $1;}
	|
        right_equal_part_min	{ $$ = $1; }
	|
	condition		{ $$ = $1; }
	|
	constructor		{ $$ = $1; }
	;

constructor: 
	id_exp PARENS_START constructor_params PARENS_END	{ $$ = mkcmdnode($1, $3, NULL, OP_CONSTRUCTOR); }
	;

constructor_params:
	constructor_parameter					{ $$ = mkcmdnode($1, NULL, NULL, OP_CONSTRUCTOR_PARAMS); }
	|
	constructor_params COMMA constructor_parameter		{ $$ = mkcmdnode($1, $3, NULL, OP_CONSTRUCTOR_PARAMS); }	
	;

constructor_parameter:
	id_exp COLON right_equal_part_min			{ $$ = mkcmdnode($1, $3, NULL, OP_CONSTRUCTOR_PARAMETER); }
	;

string_exp:
	STRING		{ $$ = mknode(NULL, NULL, NULL, yylval.strval); }
	;

plus_exp:
	math_exp				{ $$ = $1; }
	|
	string_exp				{ $$ = $1; }
	|
	function_call				{ $$ = $1; }
	;

math_exp:
	math_unary_exp				{ $$ = $1; }
	|
	operand				    	{ $$ = $1; }
	|
	PARENS_START operand PARENS_END 	{ $$ = mkcmdnode($2, NULL, NULL, OP_BRACKETS); }
	|
	PARENS_START math_exp PARENS_END 	{ $$ = mkcmdnode($2, NULL, NULL, OP_BRACKETS); }
	|
	math_exp OPERATOR_MINUS math_exp 	{ $$ = mkcmdnode($1, $3, NULL, OP_MINUS); }
	|
	plus_exp OPERATOR_PLUS plus_exp 	{ $$ = mkcmdnode($1, $3, NULL, OP_PLUS); }
	|
	math_exp OPERATOR_MULTIPLY math_exp 	{ $$ = mkcmdnode($1, $3, NULL, OP_MULTIPLY); }
	|
	math_exp OPERATOR_DIVISION math_exp 	{ $$ = mkcmdnode($1, $3, NULL, OP_DIVISION); }
	|
	math_exp OPERATOR_MATH_AND math_exp	{ $$ = mkcmdnode($1, $3, NULL, OP_MATH_AND); }
	|
	math_exp OPERATOR_MATH_OR math_exp 	{ $$ = mkcmdnode($1, $3, NULL, OP_MATH_OR); }
	|
	math_exp OPERATOR_MATH_XOR math_exp 	{ $$ = mkcmdnode($1, $3, NULL, OP_MATH_XOR); }
	;

operand:
        number_exp	{ $$ = $1; }
	|
        fp_number_exp	{ $$ = $1; }
	|
        hex_number_exp	{ $$ = $1; }
	|
	SYMBOL		{ $$ = mknode(NULL, NULL, NULL, yylval.strval); }
	|
	var		{ $$ = $1; }
	|
	array_element	{ $$ = $1; }
	;

boolean_var:
	VAL_TRUE							{ $$ = mkcmdnode(NULL, NULL, NULL, VALUE_TRUE); }
	|
	VAL_FALSE							{ $$ = mkcmdnode(NULL, NULL, NULL, VALUE_FALSE); }
	;

condition:
	right_equal_part_min                           			{ $$ = $1;}
	| 
	condition condition_operator condition				{ $$ = mkcmdnode($1, $2, $3, CMP_COMPLEX); }
	|
	right_equal_part_min COMPARE_EQUAL right_equal_part_min		{ $$ = mkcmdnode($1, $3, NULL, CMP_EQUAL); }
	|
	right_equal_part_min COMPARE_NOT_EQUAL right_equal_part_min	{ $$ = mkcmdnode($1, $3, NULL, CMP_NOT_EQUAL); }
	| 
	right_equal_part_min COMPARE_LESS right_equal_part_min		{ $$ = mkcmdnode($1, $3, NULL, CMP_LESS); }
	| 
	right_equal_part_min COMPARE_MORE right_equal_part_min		{ $$ = mkcmdnode($1, $3, NULL, CMP_MORE); }
	|                                                           
	right_equal_part_min COMPARE_LESS_EQUAL right_equal_part_min	{ $$ = mkcmdnode($1, $3, NULL, CMP_LESS_EQUAL); }
	| 
	right_equal_part_min COMPARE_MORE_EQUAL right_equal_part_min	{ $$ = mkcmdnode($1, $3, NULL, CMP_MORE_EQUAL); }
	|
	PARENS_START condition PARENS_END				{ $$ = $2; }
	;

condition_operator:
	OR		{ $$ = mkcmdnode(NULL, NULL, NULL, OP_OR); }
	|
	AND		{ $$ = mkcmdnode(NULL, NULL, NULL, OP_AND); }
	;

if_operator: 
	IF condition newlines		{ $$ = mkcmdnode($2, $3, NULL, OP_IF); }
	;

one_line_if_operator: 
	condition QUESTION_SIGN right_equal_part COLON right_equal_part	{ $$ = mkcmdnode($1, $3, $5, OP_SHORT_IF); }
	|
	PARENS_START one_line_if_operator PARENS_END			{ $$ = $2; }
	;

else_operator: 			
	ELSE newlines			{ $$ = mkcmdnode($2, NULL, NULL, OP_ELSE); }
	;

break_operator: 			
	BREAK newlines			{ $$ = mkcmdnode($2, NULL, NULL, OP_BREAK); }
	;

continue_operator: 			
	CONTINUE newlines		{ $$ = mkcmdnode($2, NULL, NULL, OP_CONTINUE); }
	;

while_operator: 
	WHILE condition newlines	{ $$ = mkcmdnode($2, $3, NULL, OP_WHILE); }
	;

for_operator:
	FOR initializations END_PART condition END_PART comma_commands_for { $$ = mkcmdnode($2, $4, $6, OP_FOR); }
	;

comma_commands_for:
	comma_commands newlines		{ $$ = mknode($1, $2, NULL, ""); }
	;

comma_commands: 
	command_simple					{ $$ = mkcmdnode($1, NULL, NULL, OP_COMMA); }
        | 
	comma_commands COMMA command_simple		{ $$ = mkcmdnode($1, $3, NULL, OP_COMMA); }
        ;

initializations: 
	making_equal_simple				{ $$ = mkcmdnode($1, NULL, NULL, OP_COMMA); }
        | 
	initializations COMMA making_equal_simple	{ $$ = mkcmdnode($1, $3, NULL, OP_COMMA); }
        ;

func_result_prop_exp:
	function_call props_in_call			{ $$ = mkcmdnode($1, $2, NULL, OP_FUNC_PROP); }
	;

my_prop_exp:
	ME props_in_call				{ $$ = mkcmdnode($2, NULL, NULL, OP_MY_PROP); }
	|
	ME						{ $$ = mkcmdnode(NULL, NULL, NULL, OP_MY_PROP); }
	;

base_prop_exp:
	BASE props_in_call				{ $$ = mkcmdnode($2, NULL, NULL, OP_BASE_PROP); }
	|
	BASE						{ $$ = mkcmdnode(NULL, NULL, NULL, OP_BASE_PROP); }
	;

prop_exp:
	id_exp props_in_call 				{ $$ = mkcmdnode($1, $2, NULL, OP_PROP); }
	|
	array_element props_in_call			{ $$ = mkcmdnode($1, $2, NULL, OP_PROP); }
	;

out_var_exp:
	OUT_SPEC id_exp					{ $$ = mkcmdnode($2, NULL, NULL, OUT_PARAM); }
	;

comment:
	singleline_comment_exp				{ $$ = mkcmdnode($1, NULL, NULL, OP_SINGLELINE_COMMENT); }
	|
	multiline_comment_exp newlines			{ $$ = mkcmdnode($1, $2, NULL, OP_MULTILINE_COMMENT); }
	;

singleline_comment_exp:
	SINGLELINE_COMMENT				{ $$ = mknode(NULL, NULL, NULL, yylval.strval); }
	;

multiline_comment_exp:
	MULTILINE_COMMENT				{ $$ = mknode(NULL, NULL, NULL, yylval.strval); }
	;

foreach_operator:
	FOREACH id_exp IN_CONSTRUCT id_exp newlines	{ $$ = mkcmdnode($2, $4, $5, OP_FOREACH); }
	;

cpp_declaration:
	cpp_exp newlines				{ $$ = mkcmdnode($1, $2, NULL, OP_CPP_DECL); }
	;

cpp_operator:
	cpp_exp newlines				{ $$ = mkcmdnode($1, $2, NULL, OP_CPP); }
	;

cpp_exp:
	CPP 						{ $$ = mknode(NULL, NULL, NULL, yylval.strval); }
	;

method_parameter_annotation:
	DOG id_exp newlines method_annotation_properties			{ $$ = mkcmdnode($2, $4, NULL, OP_PARAM_ANNOTATION); }
	|
	DOG OPERATOR_RETURN newlines method_annotation_properties		{ $$ = mkcmdnode(NULL, $4, NULL, OP_PARAM_ANNOTATION); }	
	;

method_annotation_properties:
	code_level3_exp id_exp COLON annotation_value newlines					{ $$ = mkcmdnode($2, $4, NULL, OP_PARAM_ANNOTATION_PROP); }
	|
	code_level3_exp id_exp COLON annotation_value newlines method_annotation_properties	{ $$ = mkcmdnode($2, $4, $6, OP_PARAM_ANNOTATION_PROP); }
	;

function_parameter_annotation:
	DOG id_exp newlines function_annotation_properties			{ $$ = mkcmdnode($2, $4, NULL, OP_PARAM_ANNOTATION); }
	|
	DOG OPERATOR_RETURN newlines function_annotation_properties		{ $$ = mkcmdnode(NULL, $4, NULL, OP_PARAM_ANNOTATION); }	
	;

function_annotation_properties:
	code_level_exp id_exp COLON annotation_value newlines					{ $$ = mkcmdnode($2, $4, NULL, OP_PARAM_ANNOTATION_PROP); }
	|
	code_level_exp id_exp COLON annotation_value newlines function_annotation_properties	{ $$ = mkcmdnode($2, $4, $6, OP_PARAM_ANNOTATION_PROP); }
	;

annotation_value:
	string_exp							{ $$ = $1; }
	|
	number_exp							{ $$ = $1; }
	|
	annotation_values						{ $$ = $1; }
	;

annotation_values:
	id_exp								{ $$ = mkcmdnode($1, NULL, NULL, OP_PARAM_ANNOTATION_PROP_VALUE); }
	|
	id_exp annotation_values					{ $$ = mkcmdnode($1, $2, NULL, OP_PARAM_ANNOTATION_PROP_VALUE); }
	;