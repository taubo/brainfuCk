#include <stdlib.h>
#include <stdio.h>
#include "brainfuCk.h"

typedef struct {

} simple_bf_interpreter_t;

static void simple_bf_interpreter_run(bf_interpreter_t *self, char *program)
{
    printf("simple_bf_interpreter_run\n");

    simple_bf_interpreter_t *inter = self->interpreter_p_data;
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
