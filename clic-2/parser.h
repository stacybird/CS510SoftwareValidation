
#include "list.h"
#include "pstack.h"

#ifndef PARSER_H
#define PARSER_H

#define ERRN 14

typedef enum parser_error {
	pe_ok,
	pe_bug,
	pe_noparser,
	pe_nomemory,
	pe_invalidname,
	pe_invalidconst,
	pe_constarrfull,
	pe_nameconflict,
	pe_unknownid,
	pe_syntax,
	pe_stack_overflow,
	pe_toomanyconst,
	pe_exprtoocomp,
	pe_readonly,
} PARSER_ERROR;

typedef enum expr_item_type {
	unknownid,
	variable,
	function,
	constant,
} ITEM_TYPE;

typedef struct expr_item {
	ITEM_TYPE type;
	char *name;
	void *loc;
	int argnum; /* for functions */
	int readonly; /* for constants like 'e' and 'PI'*/
} EXPR_ITEM;

typedef EXPR_ITEM VAR;
typedef EXPR_ITEM FUNC;
typedef EXPR_ITEM CONST;
typedef LIST *EXPRLISTP;

typedef struct expr {
	char *infix_str;
	EXPRLISTP postfix_list;
} EXPR;

typedef struct parser {
	PARSER_ERROR error;
	LIST *var_list;
	LIST *fun_list;
	LIST *expr_list;
	LIST *const_list;
	
	double *const_arr;
	int last_const;
	int maxconst;
	int const_nmlen;
	
	double *fargs;
	int maxfargs;
	
	double *imres;
	int maxires;
	
	PSTACK *stack;
} PARSER;

PARSER_ERROR parser_error(PARSER *p);
char *parser_errorstr(PARSER_ERROR ercode);

PARSER *parser_create(int stack_size, int maxconst, int maxfargs, int maxires);
/*
	stack_size : stack size;
	maxconst   : maximal number of constants
	maxfargs   : maximal number of function arguments
	maxires    : maximal number of intermidiate results
		     (during expression evaluation)
*/

VAR *parser_create_var(PARSER *parser, char *varname, double *var, int *exists);
void parser_print_varlist(PARSER *parser);
void parser_print_funclist(PARSER *parser);
void parser_print_constlist(PARSER *parser);

FUNC *parser_create_func(PARSER *parser, char *funcname, int argnum, double (*f)(double *args));
FUNC *parser_create_func_nocheck(PARSER *parser, char *funcname, int argnum, double (*f)(double *args));

EXPR *parser_create_expr(PARSER *parser, char *expr_str);
void parser_print_expr_infix(PARSER *parser, EXPR *ex);
void parser_print_expr_postfix(PARSER *parser, EXPR *ex);
double parser_eval_expr(PARSER *parser, EXPR *ex);
void parser_delete(PARSER *parser);

#endif
