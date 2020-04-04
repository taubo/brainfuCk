#include <assert.h>
#include <string.h>
#include <stdio.h>

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

int main(void)
{
	char *par_ok = "<<<<[[[-----]--+++]++]>\0";
	char *par_too_much_open = "<<<<[[[-------+++]++]>\0";
	char *par_too_much_closed = "<<<<[-[-----]--+++]++]>";

	printf("check_match EXPECT ok: %d\n", check_match(par_ok, strlen(par_ok)));
	printf("check_match EXPECT positive: %d\n", check_match(par_too_much_open, strlen(par_too_much_closed)));
	printf("check_match EXPECT negative: %d\n", check_match(par_too_much_closed, strlen(par_too_much_closed)));

	return 0;
}
