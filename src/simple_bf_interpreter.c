#include <stdlib.h>
#include <stdio.h>
#include "brainfuCk.h"
#include "simple_stack.h"
#include "log.h"

typedef struct {

} simple_bf_interpreter_t;

static void simple_bf_interpreter_run(bf_interpreter_t *self, char *program, bf_error_t *error)
{
    printf("simple_bf_interpreter_run\n");

    error->error_code = 0;

    simple_bf_interpreter_t *inter = self->interpreter_p_data;

    sstack *parentheses_stack = sstack_new(100, sizeof(char));
    if (!parentheses_stack) {
        error->error_code = -1;
        return;
    }

    int i = 0;
    char symbol;
    char elem = 0;
    while ((symbol = program[i]) != '\0') {
        if (symbol == '[') {
            /* log_debug("["); */
            if (sstack_push(parentheses_stack, &symbol) != 0) {
                goto exit_err;
            }
        } else if (symbol == ']') {
            /* log_debug("]"); */
            if (sstack_pop(parentheses_stack, &elem) != 0) {
                goto exit_err;
            }
        }
        i++;
    }
    if (sstack_pop(parentheses_stack, &elem) == 0) {
        /* log_debug("elem: %c", elem); */
        goto exit_err;
    }

    goto exit_ok;

exit_err:
    error->error_code = -1;
exit_ok:
    free(parentheses_stack);
}

int simple_bf_interpreter_new(bf_interpreter_t *interpreter)
{
    simple_bf_interpreter_t *priv;

    if ((priv = malloc(sizeof(simple_bf_interpreter_t))) == NULL)
            return -1;

    interpreter->run = simple_bf_interpreter_run;
    interpreter->interpreter_p_data = priv;

    return 0;
}
