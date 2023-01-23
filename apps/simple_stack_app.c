#include <stdio.h>
#include <stdlib.h>
#include "simple_stack.h"

typedef struct {
	int id;
	int position;
} test_struct;

int main(void)
{
	test_struct elem;
	test_struct peek;
	test_struct pop;

	sstack *stack;
	if ((stack = sstack_new(10, sizeof(test_struct))) == NULL) {
		printf("allocation error\n");
	}
	for (int i = 0; i < 10; ++i) {
        elem.id = rand();
        elem.position = i;
		if (sstack_push(stack, &elem) < 0) {
			printf("error trying to allocate element #%d\n", i);
		}
        sstack_peek(stack, &peek);
        printf("peek: id: %d - position: %d\n", peek.id, peek.position);
	}

    while (sstack_pop(stack, &pop) == 0) {
        printf("pop: id: %d - position: %d\n", pop.id, pop.position);
    }

	return 0;
}
