#ifndef SIMPLE_STACK_H
#define SIMPLE_STACK_H

typedef struct {
	void *data;
	size_t size;
} data;

typedef struct {
	// data *data;
	void *data;
	struct ss_elem *prev;
} ss_elem;

typedef struct {
	int size;
	int capacity;
	ss_elem *elems;
	ss_elem *peek_elem;
} sstack;

sstack *sstack_new(size_t capacity);
int sstack_push(sstack *self, ss_elem *elem);
int sstack_pop(sstack *self, ss_elem *elem);
int sstack_peek(sstack *self, ss_elem *elem);

#endif
