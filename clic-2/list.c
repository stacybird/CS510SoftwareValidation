
#include <stdlib.h>
#include "list.h"

LIST *list_create()
{
	LIST *list;
	list = (LIST*)malloc(sizeof(LIST));
	if (!list)
		return NULL;
	list->items = 0;
	list->top = NULL;
	list->last = NULL;
	return list;
}

LIST_ITEM *list_add(LIST *list, void *data)
{
	LIST_ITEM *p;

	if (!list)
		return NULL;
	
	p = (LIST_ITEM*)malloc(sizeof(LIST_ITEM));
	if (!p)
		return NULL;

	p->next = NULL;
	p->data = data;
	
	if (!list->top) {
		list->top = p;
	} else {
		list->last->next = p;
	};

	list->last = p;
	list->items++;
	return p;
}

void list_foreach(LIST *list, int (*dothis)(void*))
{
	LIST_ITEM * p;
	
	p = list->top;
	while (p) {
		if (dothis)
			dothis(p->data);
		p = p->next;
	}
}

void *list_find_first(LIST *list, void *sample, int (*match)(void*, void*))
{
	LIST_ITEM * p;
	int m = 0;

	p = list->top;
	while (p) {
		m = match(sample, p->data);
		if (m)
			break;
		p = p->next;
	}
	return (m)?(p->data):NULL;
}

void *list_find_by_addr(LIST *list, void *addr)
{
	LIST_ITEM * p;

	p = list->top;
	while (p) {
		if (addr == p->data)
			break;
		p = p->next;
	}
	return (p)?(p->data):NULL;
}

void list_delete(LIST *list, void (*delete_item)(void *data))
{
	LIST_ITEM *p, *q;
	p = list->top;

	while (p) {
		q = p->next;
		if ((delete_item) && (p->data)) {
			delete_item(p->data);
		};
		free(p);
		p = q;
	};

	free(list);
	return;
}
