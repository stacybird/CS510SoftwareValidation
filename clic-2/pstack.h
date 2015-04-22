
#ifndef PSTACK_H
#define PSTACK_H

typedef struct pstack {
	int size;
	void **loc;
	int top;
} PSTACK;

PSTACK *pstack_create(int size);
void pstack_delete(PSTACK *stk);
void pstack_push(PSTACK *stk, void *p);
void *pstack_pop(PSTACK *stk);
void *pstack_peek(PSTACK *stk);
int pstack_empty(PSTACK *stk);
int pstack_full(PSTACK *stk);
void pstack_clear(PSTACK *stk);

void pstack_print(PSTACK *stk, void (*print_item)(void*));

#endif
