#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "simple_stack.h"

int check_match(char *input_string, unsigned size)
{
	int counter = 0;

	assert(input_string != NULL);

	for (unsigned i = 0; i < size; ++i) {
		if (input_string[i] == '[') {
			counter++;
		} else if (input_string[i] == ']') {
			counter--;
		}
	}

	return counter;
}

int check_match_stack(char *input_string, unsigned size)
{
    sstack *stack;
    stack = sstack_new(20, sizeof(unsigned));
    unsigned index = 0;
    unsigned peek = 0;
    unsigned pop = 0;

    for (unsigned i = 0; i < size; ++i) {
		if (input_string[i] == '[') {
            index = i;
            sstack_push(stack, &index);
            sstack_peek(stack, &peek);
            printf("pushed peek: %u\n", peek);
		} else if (input_string[i] == ']') {
            sstack_peek(stack, &peek);
            printf("pushed %u\n", peek);
            if (sstack_pop(stack, &index) != 0)
                return -1;
            printf("closing par found at index: %u, opened at index: %u\n", i, index);
		}
    }

    if (sstack_pop(stack, &pop))
        return 1;

    return 0;
}

int main(void)
{
	char *par_ok = "<<<<[[[-----]--+++]++]>\0";
	char *par_too_much_open = "<<<<[[[-------+++]++]>\0";
	char *par_too_much_closed = "<<<[-[-----]--+++]++]>";

	printf("check_match EXPECT ok: %d\n", check_match(par_ok, strlen(par_ok)));
	printf("check_match EXPECT positive: %d\n", check_match(par_too_much_open, strlen(par_too_much_closed)));
	printf("check_match EXPECT negative: %d\n", check_match(par_too_much_closed, strlen(par_too_much_closed)));

	printf("check_match_stack EXPECT ok: %d\n", check_match_stack(par_ok, strlen(par_ok)));
	printf("check_match_stack EXPECT positive: %d\n", check_match_stack(par_too_much_open, strlen(par_too_much_closed)));
	printf("check_match_stack EXPECT negative: %d\n", check_match_stack(par_too_much_closed, strlen(par_too_much_closed)));

	return 0;
}
