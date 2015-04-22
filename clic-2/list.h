
#ifndef LIST_H
#define LIST_H

typedef struct list_item {
	void *data;
	struct list_item *next;
} LIST_ITEM;

typedef struct list {
	LIST_ITEM *top;
	LIST_ITEM *last;
	int items;
} LIST;

LIST *list_create();
LIST_ITEM *list_add(LIST *list, void *data);
void list_foreach(LIST *list, int (*dothis)(void*));
void *list_find_first(LIST *list, void *sample, int (*match)(void*, void*));
void *list_find_by_addr(LIST *list, void *addr);
void list_delete(LIST *list, void (*delete_item)(void *data));

#endif
