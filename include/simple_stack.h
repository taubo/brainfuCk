#ifndef SIMPLE_STACK_H
#define SIMPLE_STACK_H

#include <stdlib.h>

typedef struct {
	void *data;
	size_t size;
} ss_data;

typedef struct {
	ss_data *data;
} ss_elem;

typedef struct {
	int size;
	size_t capacity;
	size_t elem_size;
	void *elems;
	void *peek_elem;
} sstack;

sstack *sstack_new(size_t capacity, size_t elem_size);
void sstack_delete(sstack *self);
int sstack_push(sstack *self, void *elem);
int sstack_pop(sstack *self, void *elem);
int sstack_peek(sstack *self, void *elem);

#endif
