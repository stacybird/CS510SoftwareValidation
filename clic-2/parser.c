
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <dlfcn.h>
#include "list.h"
#include "pstack.h"
#include "parser.h"

#define LIBMNSYM 17
#define MAXCSTRLEN 8

static char cprefix[] = "_const_";

char *parser_error_msgs[ERRN] = {
	"everything in the garden's lovely",
	"oops...bug in parser",
	"invalid parser",
	"no memory",
	"invalid variable or function name",
	"invalid constant",
	"constant storage is full",
	"name conflict",
	"unknown identifier",
	"syntax error",
	"stack overflow",
	"why so many constants? :-)",
	"expression too complicated",
	"you can not change the value"
};

char *libmsyms[LIBMNSYM] = {
	"sin",
	"cos",
	"tan",
	"asin",
	"acos",
	"atan",
	"exp",
	"log",
	"log10",
	"sqrt",
	"cbrt",
	"sinh",
	"cosh",
	"tanh",
	"erf",
	"erfc",
	"tgamma"
};

char *mfname[LIBMNSYM] = {
	"sin",
	"cos",
	"tg",
	"arcsin",
	"arccos",
	"arctg",
	"exp",
	"ln",
	"lg",
	"sqrt",
	"cbrt",
	"sh",
	"ch",
	"th",
	"erf",
	"erfc",
	"G"
};

static double e;
static double PI;

/* helping functions */

int check_item_name(void *v1, void *v2);
int check_item_val(void *v1, void *v2);
int check_item_addr(void *v1, void *v2);

EXPR_ITEM *find_item_by_name(LIST *list, char *vn);
EXPR_ITEM *find_item_by_val(LIST *list, double vv);
EXPR_ITEM *find_item_by_addr(LIST *list, double *loc);

PARSER_ERROR valid_expr(PARSER*, EXPR*);

void delete_expr_item(void *dp);
void delete_expr(void *ex);

int check_item_name(void *v1, void *v2)
{
	EXPR_ITEM *var1 = (EXPR_ITEM*)v1;
	EXPR_ITEM *var2 = (EXPR_ITEM*)v2;
	
	int cr = strcmp(var1->name, var2->name);
	
	return (cr == 0)?(1):(0);
}

int check_item_val(void *v1, void *v2)
{
	EXPR_ITEM *var1 = (EXPR_ITEM*)v1;
	EXPR_ITEM *var2 = (EXPR_ITEM*)v2;
	return (*(double*)var1->loc == *(double*)var2->loc);
}

int check_item_addr(void *v1, void *v2)
{
	EXPR_ITEM *var1 = (EXPR_ITEM*)v1;
	EXPR_ITEM *var2 = (EXPR_ITEM*)v2;
	return (var1->loc == var2->loc);
}

EXPR_ITEM *find_item_by_name(LIST *list, char *vn)
{
	EXPR_ITEM sei;
	sei.name = vn;
	
	return (EXPR_ITEM*)list_find_first(list, (void*)&sei, check_item_name);
}

EXPR_ITEM *find_item_by_val(LIST *list, double vv)
{
	EXPR_ITEM sei;
	double v = vv;
	sei.loc = &v;
	
	return (EXPR_ITEM*)list_find_first(list, (void*)&sei, check_item_val);
}

EXPR_ITEM *find_item_by_addr(LIST *list, double *loc)
{
	EXPR_ITEM sei;
	sei.loc = loc;
	
	return (EXPR_ITEM*)list_find_first(list, (void*)&sei, check_item_addr);
}

int const_exists_by_val(PARSER *p, double c)
{
	int k, match;
	for (k = 0; k <= p->last_const; k++) {
		match = (p->const_arr[k] == c);
		if (match)
			break;
	};
	return match;
}

void delete_expr_item(void*dp)
{
	EXPR_ITEM *ei = (EXPR_ITEM*)dp;
	if (ei->type == constant) {
		free(ei->name);
	};
	free(ei);
}

/* end of helping functions */


/*
	+, -, *, /, ^
*/

double p_add(double *a)
{
	return a[0]+a[1];
}

double p_sub(double *a)
{
	return a[0]-a[1];
}

double p_mul(double *a)
{
	return a[0]*a[1];
}

double p_div(double *a)
{
	return a[0]/a[1];
}

double p_pow(double *a)
{
	return pow(a[0],a[1]);
}

double p_neg(double *a)
{
	return -a[0];
}

double p_nul(double *a)
{
	return a[0];
}

PARSER_ERROR parser_error(PARSER *p)
{
	if (!p)
		return pe_noparser;
	return p->error;
}

char *parser_errorstr(PARSER_ERROR ercode)
{
	if (ercode == pe_ok)
		return NULL;
	return parser_error_msgs[ercode];
}

PARSER *parser_create(int stack_size, int maxconst, int maxfargs, int maxires)
{
	PARSER *parser;
	int k;
	void* prog;
	double (*libmsym)(double*);
	char *dlerr;
	char test[MAXCSTRLEN];
	int iii;
	
	parser = (PARSER*)malloc(sizeof(PARSER));
	if ( !parser ) {
		return NULL;
	};

	parser->error      = pe_ok;
	parser->var_list   = NULL;
	parser->fun_list   = NULL;
	parser->expr_list  = NULL;
	parser->const_list = NULL;
	parser->const_arr  = NULL;
	parser->last_const = 0;
	parser->maxconst   = maxconst;
	parser->fargs      = NULL;
	parser->maxfargs   = maxfargs;
	parser->imres      = NULL;
	parser->maxires    = maxires;
	parser->stack      = NULL;
	
	if (snprintf(test, MAXCSTRLEN, "%d", maxconst) == -1) {
		parser->error = pe_toomanyconst;
		goto clean_up;
	};
	parser->const_nmlen = strlen(test)+strlen(cprefix);
	
	parser->var_list = list_create();
	if (!parser->var_list) {
		parser->error = pe_nomemory;
		goto clean_up;
	};

	parser->fun_list = list_create();
	if (!parser->fun_list) {
		parser->error = pe_nomemory;
		goto clean_up;
	};

	parser->const_list = list_create();
	if (!parser->const_list) {
		parser->error = pe_nomemory;
		goto clean_up;
	};

	parser->const_arr = (double*)malloc(maxconst*sizeof(double));
	if (!parser->const_arr) {
		parser->error = pe_nomemory;
		goto clean_up;
	};
	
	parser->imres = (double*)malloc(maxires*sizeof(double));
	if (!parser->imres) {
		parser->error = pe_nomemory;
		goto clean_up;
	};
	
	parser->fargs = (double*)malloc(maxfargs*sizeof(double));
	if (!parser->fargs ){
		parser->error = pe_nomemory;
		goto clean_up;
	};

	
	for (k = 0; k < maxconst; k++) {
		parser->const_arr[k] = 1;
	};
	
	parser->expr_list = list_create();
	if (!parser->expr_list) {
		parser->error = pe_nomemory;
		goto clean_up;
	}

	parser->stack = pstack_create(stack_size);
	if (!parser->stack) {
		parser->error = pe_nomemory;
		goto clean_up;
	}

	iii = 1;
	/* adding some built-in variables */
	if (!parser_create_var(parser, "e", &e, &iii)) {
		parser->error = pe_nomemory;
		goto clean_up;
	};
	e = exp(1);

	iii = 1;
	if (!parser_create_var(parser, "PI", &PI, &iii) ) {
		parser->error = pe_nomemory;
		goto clean_up;
	};
	PI = acos(-1);

	/* adding functions */	
	if (!parser_create_func_nocheck(parser, "+", 2, p_add)) {
		parser->error = pe_nomemory;
		goto clean_up;
	};

	if (!parser_create_func_nocheck(parser, "-", 2, p_sub)) {
		parser->error = pe_nomemory;
		goto clean_up;
	};

	if (!parser_create_func_nocheck(parser, "*", 2, p_mul)) {
		parser->error = pe_nomemory;
		goto clean_up;
	};

	if (!parser_create_func_nocheck(parser, "/", 2, p_div)) {
		parser->error = pe_nomemory;
		goto clean_up;
	};

	if (!parser_create_func_nocheck(parser, "^", 2, p_pow)) {
		parser->error = pe_nomemory;
		goto clean_up;
	};

	if (!parser_create_func_nocheck(parser, "(", 0, NULL)) {
		parser->error = pe_nomemory;
		goto clean_up;
	};

	if (!parser_create_func_nocheck(parser, ")", 0, NULL)) {
		parser->error = pe_nomemory;
		goto clean_up;
	};

	if (!parser_create_func_nocheck(parser, "neg", 1, p_neg)) {
		parser->error = pe_nomemory;
		goto clean_up;
	};

	if (!parser_create_func_nocheck(parser, "nul", 1, p_nul)) {
		parser->error = pe_nomemory;
		goto clean_up;
	};

	/* adding functions from libm */
	prog = dlopen(NULL, RTLD_LAZY);
	if (!prog)
		return parser;
	
	for (k = 0; k < LIBMNSYM; k++) {
		libmsym = dlsym(prog, libmsyms[k]);
		if ((dlerr = dlerror()) == NULL) {
			parser_create_func_nocheck(parser, mfname[k], -1, libmsym);
		};
	};

	dlclose(prog);
	return parser;
	
clean_up:

	if (parser->stack)
		pstack_delete(parser->stack);
	if (parser->imres)
		free(parser->imres);
	if (parser->fargs)
		free(parser->fargs);
	if (parser->const_arr)
		free(parser->const_arr);
	
	/* all list except function list are empty */
	if (parser->expr_list)
		list_delete(parser->expr_list, NULL);
	if (parser->var_list)
		list_delete(parser->var_list, NULL);
	if (parser->const_list)
		list_delete(parser->const_list, NULL);
	
	if (parser->fun_list)
		list_delete(parser->fun_list, delete_expr_item);
	
	free(parser);
	return NULL;	
}

VAR* parser_create_var(PARSER*parser, char *varname, double *var, int *exists)
{
	VAR *newvar;
	VAR *oldvar;
	int k;
	int readonly = *exists; /* save the value*/
	
	if (!parser)
		return NULL;
	*exists = 0;

	parser->error = pe_ok; 

	if (!(isascii(varname[0]) && isalpha(varname[0]))) {
		parser->error = pe_invalidname;
		return NULL;
	};
	
	for (k = 1; k < strlen(varname); k++) {
		if (!((isascii(varname[k]) && isalnum(varname[k])) || varname[k]=='_')) {
			parser->error = pe_invalidname;
			return NULL;
		};
	};

	/* already exists? */
	oldvar = (VAR*)find_item_by_name(parser->var_list, (void*)varname);
	if (oldvar) {
		*exists = 1;
		if (oldvar->readonly) {
			parser->error = pe_readonly;
			return NULL;
		}
		return oldvar;
	}

/*
	oldvar = (VAR*)find_item_by_addr(parser->var_list, var);
	if (oldvar) {
	};
*/

	/* also look it up in constant and function lists */
	oldvar = (VAR*)find_item_by_name(parser->const_list, (void*)varname);
	if (oldvar) {
		*exists = 1;
		parser->error = pe_nameconflict;
		return NULL;
	};
/*
	oldvar = (VAR*)find_item_by_addr(parser->const_list, var);
	if (oldvar) {
	}
*/
	oldvar = (VAR*)find_item_by_name(parser->fun_list, (void*)varname);
	if (oldvar) {
		*exists = 1;
		parser->error = pe_nameconflict;
		return NULL;
	};
/*
	oldvar = (VAR*)find_item_by_addr(parser->const_list, var);
	if ( oldvar ) {
	}
*/	
	newvar = (VAR*)malloc(sizeof(VAR));
	if (!newvar) {
		parser->error = pe_nomemory;
		return NULL;
	};
	
	newvar->name = varname;
	newvar->type = variable;
	newvar->loc  = var;
	newvar->argnum = 0;
	newvar->readonly = readonly;
	
	if (!list_add(parser->var_list, (void*)newvar)) {
		free(newvar);
		parser->error = pe_nomemory;
		return NULL;
	}

	return newvar;
}


/* for internal use only! */
VAR* parser_addconst(PARSER*parser, char *cname, double *c)
{
	VAR * newvar;

	newvar = (VAR*)malloc(sizeof(VAR));
	if (!newvar) return NULL;

	newvar->name = cname;
	newvar->type = constant;
	newvar->loc  = c;
	newvar->argnum = 0;
	newvar->readonly = 1;

	if (!list_add(parser->const_list, (void*)newvar)) {
		free(newvar);
		return NULL;
	}

	return newvar;
}

int print_item(void*v)
{
	VAR * var = (VAR*)v;
	printf("%s", var->name);
	if (var->type != function ) {
		printf(" = %f", *(double*)var->loc);
		if (var->readonly) {
			printf(" (readonly)\n");
		} else {
			printf("\n");
		}
	} else {
		printf("\n");
	};
	return 0;
}

void parser_print_varlist(PARSER*parser)
{
	list_foreach(parser->var_list, print_item);
}

void parser_print_funclist(PARSER*parser)
{
	list_foreach(parser->fun_list, print_item);
}

void parser_print_constlist(PARSER*parser)
{
	list_foreach(parser->const_list, print_item);
}

int print_expr_item(void*v)
{
	EXPR_ITEM * ei = (EXPR_ITEM*)v;
	if (ei->type != constant) {
		printf("%s ", ei->name);
	} else {
		printf("%f ", *(double*)ei->loc);
	};
	return 0;
}

void parser_print_expr_infix(PARSER*parser, EXPR *ex)
{
	
	if (!list_find_by_addr(parser->expr_list, ex)) {
		return;
	};
	
	list_foreach(ex->postfix_list, print_expr_item);
	printf("\n");
}

void parser_print_expr_postfix(PARSER*parser, EXPR *ex)
{
	
	if (!list_find_by_addr(parser->expr_list, ex)) {
		return;
	};
	
	list_foreach(ex->postfix_list, print_expr_item);
	printf("\n");
}

void print_stack_item(void *data)
{
	printf("%s ", (char*)data);
}

void print_stack(PSTACK*stk)
{
	printf("stack: ");
	pstack_print(stk, print_stack_item);
	printf("\n");	
}

FUNC *parser_create_func_nocheck(PARSER *parser, char *funcname, int argnum, double (*f)(double *args))
{
	FUNC *newf;
	FUNC *oldf;

	/* already exists? */
	oldf = (FUNC*)find_item_by_name(parser->fun_list, (void*)funcname);

	if (oldf) {
		return oldf;
	}

	newf = (FUNC*)malloc(sizeof(FUNC));
	if (!newf) return NULL;
	
	newf->name = funcname;
	newf->type = function;
	newf->loc  = f;
	newf->argnum = argnum;
	
	if (!list_add(parser->fun_list, (void*)newf)) {
		free(newf);
		return NULL;
	}
	
	return newf;
}

FUNC *parser_create_func(PARSER *parser, char *funcname, int argnum, double (*f)(double *args) )
{
	FUNC *newf;
	FUNC *oldf;
	int k;

	if (!parser) {
		return NULL;
	};
	
	parser->error = pe_ok; 

	if (!(isascii(funcname[0]) && isalpha(funcname[0]))) {
		parser->error = pe_invalidname;
		return NULL;
	};
	
	for (k = 1; k < strlen(funcname); k++) {
		if (!((isascii(funcname[k]) && isalnum(funcname[k])) || funcname[k]=='_')) {
			parser->error = pe_invalidname;
			return NULL;
		};
	};

	/* already exists? */
	oldf = (FUNC*)find_item_by_name(parser->fun_list, (void*)funcname);
	if (oldf) {
		return oldf;
	};


	oldf = (FUNC*)find_item_by_name(parser->var_list, (void*)funcname);
	if (oldf) {
		parser->error = pe_nameconflict;
		return NULL;
	};

	oldf = (FUNC*)find_item_by_name(parser->const_list, (void*)funcname);
	if (oldf) {
		parser->error = pe_nameconflict;
		return NULL;
	};
	
	newf = (FUNC*)malloc(sizeof(FUNC));
	if (!newf) {
		parser->error = pe_nomemory;
		return NULL;
	}
	
	newf->name = funcname;
	newf->type = function;
	newf->loc  = f;
	newf->argnum = argnum;
	
	if (!list_add(parser->fun_list, (void*)newf)) {
		free(newf);
		parser->error = pe_nomemory;
		return NULL;
	}
	
	return newf;
}

/*
ITEM_TYPE token_type(PARSER*parser, char*tok)
{
	EXPR_ITEM ei;
	
	if (isdigit(tok[0]) || (tok[0] == '.')) 
		return constant;
		
	if ((tok[0] == '(') || (tok[0] == ')')) 
		return function;
	
	ei.name = tok;
		
	if (list_find_first(parser->var_list, (void*)&ei, check_item_name))
		return variable;
		
	if (list_find_first(parser->fun_list, (void*)&ei, check_item_name))
		return function;
		
	return unknownid;
}
*/

int priority(char op1, char op2)
{
	if (op1 == ')')
		return -1;

	if (op1 == '(' )
		return 0;

	if ((op1 != ')') && (op2 == '(') )
		return 0;

	if (isalpha(op2))
		return 0; /* function */

	if (op2 == '^')
		return 0;

	if ((( op1 == '+') || (op1 == '-')) && ( (op2 == '*') || (op2 == '/')))
		return 0;

	return 1;
}

char *delims = "+-()^*/,";

EXPR_ITEM* find_item(PARSER*p, char*name)
{
	EXPR_ITEM * ei;

	ei = find_item_by_name(p->var_list, name);
	if (ei) 
		return ei;

	ei = find_item_by_name(p->fun_list, name);
	if (ei) 
		return ei;

	return NULL;
}

int valid_const(char *cstr)
{
	int k;
	int valid;
	int dots;

	dots = 0;
	for (k = 0; k < strlen(cstr); k++) {
		valid = (isdigit(cstr[k]) || (cstr[k] == '.'));
		if (cstr[k] == '.')
			dots++;
		if (dots > 1)
			valid = 0;
		if (!valid )
			break;
	};
	return valid;
}

EXPR* parser_create_expr(PARSER *parser, char *expr_str)
{
	EXPR *e = NULL;
	char *e_str = NULL;
	char *tmp = NULL, *tmpsave = NULL;
	char *token = NULL;
	int len, m, k, olen;
	char c;
	double v;
	char ariff[2] = " ";
	char *neg = "neg";
	char *nul = "nul";
	EXPR_ITEM *item = NULL;
	int unop;
	char *cname = NULL;
	char cn[MAXCSTRLEN];

	if (!parser)
		return NULL;
	if (!expr_str)
		return NULL;

	parser->error = pe_ok;

	e = (EXPR*)malloc(sizeof(EXPR));
	if (!e) {
		parser->error = pe_nomemory;
		goto clean_up;
	}

	e->postfix_list = list_create();
	
	if (!e->postfix_list) {
		parser->error = pe_nomemory;
		goto clean_up;
	};

	/* how many spaces we have? */
	{
		int spaces = 0;
		int k;
		olen = len = strlen(expr_str);
		
		for (k = 0; k < olen; k++) {
			if (expr_str[k] == ' ') spaces++;
		};
		len -= spaces;
	};
	
	e_str = (char*)malloc(len + 1); /* 1 for terminating \0 */
	if (!e_str) { 
		parser->error = pe_nomemory;
		goto clean_up;
	};

	tmp = malloc(len + 1); /* 1 for terminating \0 */
	tmpsave = tmp;
	if (!tmp) { 
		parser->error = pe_nomemory;
		goto clean_up;
	};
	
	e_str[len] = '\0';
	
	m = 0;
	for (k = 0; k < olen; k++) {
		if (expr_str[k] != ' ') {
			e_str[m] = expr_str[k];
			m++;
		};
	};
	
/*
	checking brakets
*/
	pstack_clear(parser->stack);
	
	for (k = 0; k < len; k++) {
		if (e_str[k] == '(') { 
			if (pstack_full(parser->stack)) {
				parser->error = pe_exprtoocomp;
				goto clean_up;
			};
			pstack_push(parser->stack,tmp);
		};
		if (e_str[k] == ')') { 
			if (pstack_empty(parser->stack)) {
				parser->error = pe_syntax;
				goto clean_up;
			};
			pstack_pop(parser->stack);
		};
	};

	if (!pstack_empty(parser->stack)) {
		parser->error = pe_syntax;
		goto clean_up;
	};
	
	e->infix_str = e_str;
	strcpy(tmp, e_str);
	
	/* 
		parsing expression string in infix notation
		and create expression list in postfix notation
	*/
	
	k = 0; /* position of first unparsed symbol */
	m = 0; /* flag for strtok() */
	pstack_clear(parser->stack);
	
	while (k < len) {
		c = e_str[k];
		if (strchr(delims, (int)c)) {
			/* it's an operation */
			ariff[0] = c;
			token = ariff;
			
			/* check for unary minus or plus */
			if ((c == '-') || (c == '+') ) {
				unop = ((!k) || ((k) && (!isalnum(e_str[k-1])) && ( e_str[k-1] != ')')));
			};
			k++;

			if (unop) {
				if ( c == '-' ) {
					token = neg;
				};
				if ( c == '+' ) {
					token = nul;
				};
			};
		} else { /* it is a function, a variable or numeric constant */
			if (m) {
				token = strtok(NULL, delims);
			} else {
				token = strtok(tmp, delims);
				m = 1;
			};
			k += strlen(token);
		};
		
		/* check if it is constant */
		if ((token[0] == '.') || (isdigit(token[0]))) {
			EXPR_ITEM *ei;
			if (!valid_const(token)) {
				parser->error = pe_invalidconst;
				goto clean_up;
			};
			if (!sscanf(token, "%lf", &v)) {
				parser->error = pe_invalidconst;
				goto clean_up;
			};
			
			ei = find_item_by_val(parser->const_list, v);

			if (!ei) {
				cname = (char*)malloc(parser->const_nmlen);
				if (!cname) {
					parser->error = pe_nomemory;
					goto clean_up;
				};

				if (parser->last_const == parser->maxconst) {
					parser->error = pe_constarrfull;
					goto clean_up;
				};
				
				strcpy(cname, cprefix);
				sprintf(cn,"%d", parser->last_const);
				strcat(cname,cn);
				
				parser->const_arr[parser->last_const] = v;
				
				ei = parser_addconst(parser, cname, &parser->const_arr[parser->last_const]);
				parser->last_const++;
				
				if (!ei) {
					parser->error = pe_nomemory;
					goto clean_up;
				};
				
			};
			item = ei;
		} else {
			item = find_item(parser, token);
		};
		
		if (!item) {
			parser->error = pe_unknownid;
			goto clean_up;
		};
		
		switch ( item->type ) {
		
	case constant:
	case variable:
		if (!list_add(e->postfix_list, (void*)item)) {
			parser->error = pe_nomemory;
			goto clean_up;
		};
	break;
		
	case function:
		while (1) {
			EXPR_ITEM * otop;
			if (pstack_empty(parser->stack))
				break;
			otop = (EXPR_ITEM*)pstack_peek(parser->stack);
			if (!priority(otop->name[0],item->name[0]))
				break;
			otop = (EXPR_ITEM*)pstack_pop(parser->stack);
			if (!list_add(e->postfix_list, (void*)otop)) {
				parser->error = pe_nomemory;
				goto clean_up;
			};
		};
		if (item->name[0] != ')') {
//			if (pstack_full(parser->stack) ) {
//				parser->error = pe_stack_overflow;
//				goto clean_up;
//			};
			pstack_push(parser->stack, item);
		} else {
//			if (pstack_empty(parser->stack) ) {
//				parser->error = pe_syntax; /* ??? */
//				goto clean_up;
//			};
			pstack_pop(parser->stack); /* throw away '('*/
		};
	break;
	
	/* never happen */
	case unknownid:
		parser->error = pe_bug;
		goto clean_up;
	break;
		}; /* */
		
	}; /* while there are tokens */

	while (!pstack_empty(parser->stack)) {
		EXPR_ITEM * op = pstack_pop(parser->stack);
		if ( !list_add(e->postfix_list, op) ) {
			parser->error = pe_nomemory;
			goto clean_up;
		};
	};

	{
		PARSER_ERROR aaa = valid_expr(parser, e);
		if (aaa != pe_ok) {
			parser->error = aaa;
			goto clean_up;
		};
	}
	
	if (!list_add(parser->expr_list, (void*)e)) {
		parser->error = pe_nomemory;
		goto clean_up;
	};
	return e;

clean_up:

	if (tmpsave)
		free(tmpsave);
	if (e_str)
		free(e_str);
	if (e->postfix_list )
		list_delete(e->postfix_list, NULL);
	if (e)
		free(e);
	return NULL;
}

PARSER_ERROR valid_expr(PARSER *parser, EXPR *ex)
{
	double *tmp = NULL;
	LIST_ITEM *itemp;
	EXPR_ITEM *cei;
	int k;

	int argnum;

	itemp = ex->postfix_list->top;
	pstack_clear(parser->stack);

	while (itemp) {
		cei = (EXPR_ITEM*)itemp->data;
		switch (cei->type) {

		case constant:
		case variable:
			if (pstack_full(parser->stack)) {
				return pe_exprtoocomp;
			};
			pstack_push(parser->stack, tmp);
		break;

		case function:
			argnum = abs(cei->argnum);
			for (k = argnum - 1; k >= 0; k--) {
				if (pstack_empty(parser->stack)) {
					return pe_syntax;
				};
				pstack_pop(parser->stack);
			};
			if (pstack_full(parser->stack)) {
				return pe_exprtoocomp;
			};
			pstack_push(parser->stack, tmp);
		break;

		case unknownid:
			printf("BUG: unknown item id\n");
			exit(1);
		break;
		}
		itemp = itemp->next;
	};

	if (pstack_empty(parser->stack))
		return pe_syntax;
	pstack_pop(parser->stack);
	if (!pstack_empty(parser->stack))
		return pe_syntax;

	return pe_ok;
}

double parser_eval_expr(PARSER *parser, EXPR *ex)
{
	double *tmp;
	LIST_ITEM *itemp;
	EXPR_ITEM *cei;
	int k;
	double (*op)(double*);
	double (*mlop)(double);
	
	int ind, argnum;
	
	if (!parser)
		return 0;
	if (!ex)
		return 0;

	itemp = ex->postfix_list->top;
	ind = 0;

	pstack_clear(parser->stack);

	while (itemp) {
		cei = (EXPR_ITEM*)itemp->data;
		switch (cei->type) {

		case constant:
		case variable:
			/* error? */
			pstack_push(parser->stack, cei->loc);
		break;

		case function:
			argnum = abs(cei->argnum);
			for (k = argnum-1; k >= 0; k--) {
				tmp = pstack_pop(parser->stack);
				parser->fargs[k] = *(double*)tmp;
			};
			if (cei->argnum == -1) {
				mlop = cei->loc;
				parser->imres[ind] = mlop(parser->fargs[0]);
			} else {
				op = cei->loc;
				parser->imres[ind] = op(parser->fargs);
			}
			pstack_push(parser->stack, &parser->imres[ind]);
			ind++;
		break;

		case unknownid:
		break;
		}
		itemp = itemp->next;
	}
	tmp = pstack_pop(parser->stack);
	return *tmp;
}

void delete_expr(void *ex)
{
	/* what to do with char *infix_str ?*/
	EXPR *exp = (EXPR*)ex;
	LIST *exl = exp->postfix_list;
	list_delete(exl, NULL);
	/* 
	 * no need to delete items since they are
	 * in var, func and const lists
	 */
}

void parser_delete(PARSER*parser)
{
	list_delete(parser->expr_list, delete_expr);
	list_delete(parser->var_list, delete_expr_item);
	list_delete(parser->fun_list, delete_expr_item);
	list_delete(parser->const_list, delete_expr_item); 

	free(parser->const_arr);
	free(parser->imres);
	free(parser->fargs);
	pstack_delete(parser->stack);
	
	free(parser);
}
