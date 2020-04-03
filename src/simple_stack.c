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

sstack *sstack_new(size_t capacity)
{
	ss_elem *elems;
	sstack *self = NULL;

	if (capacity <= 0)
		return self;

	if ((self = malloc(sizeof(sstack))) == NULL) {
		return self;
	}

	self->capacity = capacity;
	self->size = 0;
	self->peek_elem = NULL;

	if ((elems = calloc((size_t)capacity, sizeof(ss_elem))) == NULL) {
		goto dealloc;
	}

	self->elems = elems;

	return self;

dealloc:
	free(self);

	return NULL;
}

int sstack_push(sstack *self, ss_elem *elem)
{
	if (is_full(self)) {
		return -1;
	}

	memcpy(self->elems + self->size, elem, sizeof(ss_elem));
	self->peek_elem = self->elems + self->size;

	self->size++;

	return 0;
}

int sstack_pop(sstack *self, ss_elem *elem)
{
	if (is_empty(self)) {
		return -1;
	}

	self->peek_elem.data = NULL;

	self->size--;

	return 0;
}

int sstack_peek(sstack *self, ss_elem *elem)
{
	if (is_empty(self)) {
		return -1;
	}

	memcpy(elem, self->peek_elem, sizeof(ss_elem));

	return 0;
}
