#include <stdlib.h>
#include <string.h>
#include "simple_stack.h"

// push
// pop
// peek

static int is_full(sstack *self)
{
	if (self->size == self->capacity) {
		return 1;
	} else {
		return 0;
	}
}

static int is_empty(sstack *self)
{
	if (self->size == 0) {
		return 1;
	}

	return 0;
}

sstack *sstack_new(size_t capacity, size_t elem_size)
{
	void *elems;
	sstack *self = NULL;

	if (capacity <= 0)
		return self;

	if ((self = malloc(sizeof(sstack))) == NULL) {
		return self;
	}

	self->capacity = capacity;
    self->elem_size = elem_size;
	self->size = 0;
	self->peek_elem = NULL;

	if ((elems = calloc((size_t)capacity, elem_size)) == NULL) {
		goto dealloc;
	}

	self->elems = elems;

	return self;

dealloc:
	free(self);

	return NULL;
}

int sstack_push(sstack *self, void *elem)
{
	if (is_full(self)) {
		return -1;
	}

	memcpy(self->elems + (self->size * self->elem_size), elem, self->elem_size);

	self->peek_elem = self->elems + (self->size * self->elem_size);
	self->size++;

	return 0;
}

int sstack_pop(sstack *self, void *elem)
{
	if (is_empty(self)) {
		return -1;
	}

	self->size--;
    self->peek_elem = self->elems + (self->size * self->elem_size);
    memcpy(elem, self->peek_elem, self->elem_size);

	return 0;
}

int sstack_peek(sstack *self, void *elem)
{
	if (is_empty(self)) {
		return -1;
	}

	memcpy(elem, self->peek_elem, self->elem_size);

	return 0;
}
