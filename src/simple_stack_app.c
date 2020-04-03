#include <stdio.h>
#include <stdlib.h>
#include "simple_stack.h"

typedef struct {
	int id;
	int position;
} test_struct;

int main(void)
{
	ss_elem elem;
	ss_elem peek;

	if ((elem.data = malloc(sizeof(test_struct))) == NULL) {
		printf("allocation error\n");

		return -1;
	}

	test_struct *data = elem.data;
	data->id = 5;
	data->position = 0;

	sstack *stack;
	if ((stack = sstack_new(10)) == NULL) {
		printf("allocation error\n");
	}
	sstack_push(stack, &elem);
	data->id = 6;
	sstack_push(stack, &elem);
	for (int i = 0; i < 10; ++i) {
		if (sstack_push(stack, &elem) < 0) {
			printf("error trying to allocate element #%d\n", i);
		}
	}
	sstack_peek(stack, &peek);

	test_struct *peek_data = peek.data;
	printf("id: %d - position: %d\n", peek_data->id, peek_data->position);

	return 0;
}
