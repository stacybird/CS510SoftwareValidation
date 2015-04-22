
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parser.h"

#define MAX_USER_VARS 1024
#define MAX_ID_LEN 64

struct user_var {
	char name[MAX_ID_LEN + 1];
	double val;
} vars[MAX_USER_VARS];

int slot = 0;

PARSER *p;
EXPR *expr;

int main(void)
{
	char *es = NULL;
	int k;

	for (k = 0; k < MAX_USER_VARS; k++) {
		memset(vars[k].name, 0, MAX_ID_LEN + 1);
		vars[k].val = 0;
	};

	p = parser_create(512, 128, 128, 512);
	if (!p) {
		printf("Error creating parser\n");
		return 1;
	};

	using_history();
	printf("Type '\\h' to get some help\n");

	while (1) {
		char *cp1, *cp2;
		double r;
		char *infix;

		if (es)
			free(es);
		es = readline("clic2> ");
		if (!es)
			continue;
		if (!*es)
			continue;
		add_history(es);

		/* special command */
		if ((strlen(es) == 2) && (es[0] == '\\')) {
			switch (es[1]) {
			case 'q':
				goto __end;
			case 'v': 
				parser_print_varlist(p);
				continue;
			case 'f': 
				parser_print_funclist(p);
				continue;
			case 'c': 
				parser_print_constlist(p);
				continue;
			case 'h': 
				printf("\\v - variable list\n\\f - function list\n\\q - quit\n");
				continue;
			default:
				printf("---unknown command\n");
				continue;
			}
		}

		cp1 = strchr(es, '=');
		cp2 = strrchr(es, '=');
		if (cp1 != cp2) {
			printf("---more than one '='\n");
			continue;
		}

		infix = es;

		/* we have some assignment */
		if (cp1) {
			cp1[0] = 0;
			cp1++;

			/**
			 *  TODO: add macro support
			 **/

			if (slot == MAX_USER_VARS) {
				printf("---no more space left for variables\n");
				continue;
			}
			if (strlen(es) > MAX_ID_LEN) {
				printf("---identifier is too long\n");
				continue;
			}
			memset(vars[slot].name, 0, MAX_ID_LEN + 1);
			strcpy(vars[slot].name, es);
			infix = cp1;
		}

		/* calculate the expression */
		expr = parser_create_expr(p, infix);
		if (!expr) {
			printf("---%s\n", parser_errorstr(p->error));
			continue;
		};

		/** 
		 * uncomment the line below to see expressions in postfix notation
		 */
		// parser_print_expr_postfix(p, expr);

		r = parser_eval_expr(p, expr);

		if (cp1) {
			VAR *v;
			int exists = 0;
			v = parser_create_var(p, vars[slot].name, &vars[slot].val, &exists);
			if (!v) {
				printf("---%s\n", parser_errorstr(p->error));
				continue;
			}
			*(double*)v->loc = r;
			if (!exists) /* new var */
				slot++;
		};

		printf("%.6f\n", r);
	};

      __end:
	parser_delete(p);
	return 0;
}
