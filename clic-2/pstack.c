
#include <stdlib.h>
#include "pstack.h"

PSTACK *pstack_create(int items)
{
	PSTACK *s;

	s = (PSTACK*)malloc(sizeof(PSTACK));
	if (!s)
		return NULL;

	s->size = items;
	s->loc = (void*)malloc(items*sizeof(void*));
	
	if (!s->loc) {
		free(s);
		return NULL;
	}

	s->top = s->size;
	return s;
} 

void pstack_delete(PSTACK *stk)
{
	if (!stk)
		return;
	
	free(stk->loc);
	free(stk);
}

void pstack_push(PSTACK *stk, void *p)
{
	if (!stk) return;
	
	if (!stk->top) 
		return; /* overflow */

	stk->top--;
	stk->loc[stk->top] = p;
	return; /* success */
}

void *pstack_pop(PSTACK *stk)
{
	void *p;

	if (!stk)
		return NULL;

	if (stk->top >= stk->size)
		return NULL; /* underflow, but.... */

	p = stk->loc[stk->top];
	stk->top++;
	return p;
}

int pstack_empty(PSTACK *stk)
{
	if (!stk)
		return 1;
	return (stk->top == stk->size);
}

int pstack_full(PSTACK* stk)
{
	if (!stk)
		return 1;
	return (stk->top == 0);
}

void *pstack_peek(PSTACK *stk)
{
	if (!stk)
		return NULL;
	if (stk->top >= stk->size )
		return NULL; /* underflow, but.... */
	return stk->loc[stk->top];
}

void pstack_clear(PSTACK *stk)
{
	stk->top = stk->size;
}

void pstack_print(PSTACK *stk, void (*print_item)(void*) )
{
	int k;
	
	if ((!stk) || (!print_item))
		return;
	
	for (k = stk->size-1; k >= stk->top; k--) {
		print_item(stk->loc[k]);
	}
}
