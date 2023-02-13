#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "brainfuCk.h"
#include "simple_stack.h"
#include "log.h"

#define MAX_STATE_ARRAY_SIZE    65536

typedef struct {
    char *open;
    char *closed;
} parentheses_pair;


typedef struct {
    unsigned char state_array[MAX_STATE_ARRAY_SIZE];
    unsigned char *current_position;
    unsigned char *program_pointer;
    int current_parentheses_pos;
    parentheses_pair parentheses_pointers_array[1000];
    sstack *execution_stack;
} simple_bf_interpreter_t;

static void print_state_array(simple_bf_interpreter_t *interpreter)
{
    for (int i = (MAX_STATE_ARRAY_SIZE / 2) - 20; i < (MAX_STATE_ARRAY_SIZE / 2) + 20; ++i)
        printf("\t[%d]: %02x %c\n", i, interpreter->state_array[i], interpreter->state_array[i]);
    printf("\n");
}

static void print_state(simple_bf_interpreter_t *interpreter)
{
    printf("\n=========================\n");
    printf("|\t[%5d]\t\t|\n", interpreter->current_position - interpreter->state_array);
    printf("|\t( %c | %02x )\t|\n", *interpreter->current_position, *interpreter->current_position);
    printf("=========================\n");
}

static int syntactic_check(char *program)
{
    int ret = 0;

    sstack *parentheses_stack = sstack_new(100, sizeof(char));
    if (!parentheses_stack) {
        /* error->error_code = -1; */
        return -1;
    }

    int i = 0;
    char symbol;
    char elem = 0;
    while ((symbol = program[i]) != '\0') {
        if (symbol == '[') {
            /* log_debug("["); */
            if (sstack_push(parentheses_stack, &symbol) != 0) {
                ret = -1;
                goto exit_err;
            }
        } else if (symbol == ']') {
            /* log_debug("]"); */
            if (sstack_pop(parentheses_stack, &elem) != 0) {
                ret = -1;
                goto exit_err;
            }
        }
        i++;
    }
    if (sstack_pop(parentheses_stack, &elem) == 0) {
        /* log_debug("elem: %c", elem); */
        ret = -1;
        goto exit_err;
    }

    goto exit_ok;

exit_err:
    /* error->error_code = -1; */

exit_ok:
    free(parentheses_stack);

    return ret;
}

static int pair_compar(const void *pair1, const void *pair2)
{
    parentheses_pair *p1 = (parentheses_pair *) pair1;
    parentheses_pair *p2 = (parentheses_pair *) pair2;

    if (p1->open < p2->open)
        return -1;
    else if (p1->open > p2->open)
        return 1;
    else
        return 0;
}

static int simple_bf_parse(char *program, simple_bf_interpreter_t *interpreter)
{
    int ret = 0;
    int i, j, k = 0;
    char symbol;

    sstack *parentheses_pointers_stack = sstack_new(100, sizeof(parentheses_pair));
    if (parentheses_pointers_stack == NULL) {
        return -1;
    }

    sstack *parentheses_pointers_stack_out = sstack_new(1000, sizeof(parentheses_pair));
    if (parentheses_pointers_stack_out == NULL) {
        ret = -1;
        goto exit_err;
    }

    while ((symbol = program[i]) != '\0') {
        if (symbol == '[') {
            /* log_debug("["); */
            parentheses_pair pair;
            pair.open = &program[i];

            if (sstack_push(parentheses_pointers_stack, &pair) != 0) {
                ret = -2;
            }
        } else if (symbol == ']') {
            /* log_debug("]"); */
            parentheses_pair pair;
            if (sstack_pop(parentheses_pointers_stack, &pair) != 0) {
                ret = -3;
            }
            pair.closed = &program[i];

            if (sstack_push(parentheses_pointers_stack_out, &pair) != 0) {
                ret = -4;
            }
        }
        i++;
    }

    sstack_to_array(parentheses_pointers_stack_out, interpreter->parentheses_pointers_array);
    qsort(interpreter->parentheses_pointers_array, parentheses_pointers_stack_out->size, sizeof(parentheses_pair), pair_compar);
    for (int i = 0; i < 10; ++i) {
        printf("open: %d -- closed: %d\n",
                interpreter->parentheses_pointers_array[i].open - program,
                interpreter->parentheses_pointers_array[i].closed - program);
    }

    free(parentheses_pointers_stack_out);

exit_err:

    free(parentheses_pointers_stack);

    return ret;
}

static void handle_plus(simple_bf_interpreter_t *interpreter)
{
    (*interpreter->current_position)++;
    interpreter->program_pointer++;
}

static void handle_minus(simple_bf_interpreter_t *interpreter)
{
    (*interpreter->current_position)--;
    interpreter->program_pointer++;
}

static void handle_less_than(simple_bf_interpreter_t *interpreter)
{
    /* return an error or something */
    if (interpreter->current_position == interpreter->state_array)
        return;

    interpreter->current_position--;
    interpreter->program_pointer++;
}

static void handle_greater_than(simple_bf_interpreter_t *interpreter)
{
    /* return an error or something */
    if (interpreter->current_position == interpreter->state_array + MAX_STATE_ARRAY_SIZE) {
        /* return; */
        log_info("error");
    }

    interpreter->current_position++;
    interpreter->program_pointer++;
}

static void handle_comma(simple_bf_interpreter_t *interpreter)
{
    *interpreter->current_position = getchar();
    interpreter->program_pointer++;
}

static void handle_point(simple_bf_interpreter_t *interpreter)
{
    /* printf("ptr value: %x\n", *interpreter->current_position); */
    putchar(*interpreter->current_position);
    interpreter->program_pointer++;
}

static void handle_o_bracket(simple_bf_interpreter_t *interpreter)
{
    int i = 0;
    while (interpreter->program_pointer != interpreter->parentheses_pointers_array[i].open)
        i++;

    if (*interpreter->current_position == 0) {
        interpreter->program_pointer = interpreter->parentheses_pointers_array[i].closed;
    } else {
        /* sstack_push(interpreter->execution_stack, &interpreter->parentheses_pointers_array[i]); */
        /* interpreter->program_pointer++; */
    }
    interpreter->program_pointer++;

    /* if (*interpreter->current_position == 0) { */
    /*     int curr = 0; */
    /*     sstack_peek(interpreter->execution_stack, &curr); */
    /*     interpreter->program_pointer = interpreter->parentheses_pointers_array[curr].closed; */
    /*     /1* interpreter->current_parentheses_pos--; *1/ */
    /* } else { */
    /*     interpreter->current_parentheses_pos++; */
    /*     sstack_push(interpreter->execution_stack, &interpreter->current_parentheses_pos); */
    /* } */
    /* printf("handle_o_bracket: i: %d\tcurrent_parentheses_pos: %d\n", i, interpreter->current_parentheses_pos); */
    /* interpreter->program_pointer++; */
}

static void handle_c_bracket(simple_bf_interpreter_t *interpreter)
{
    /* parentheses_pair next; */

    int i = 0;
    while (interpreter->program_pointer != interpreter->parentheses_pointers_array[i].closed)
        i++;

    if (*interpreter->current_position != 0) {
        /* sstack_pop(interpreter->execution_stack, &next); */
        /* sstack_peek(interpreter->execution_stack, &next); */
        /* interpreter->program_pointer = next.open; */
        interpreter->program_pointer = interpreter->parentheses_pointers_array[i].open;
    } else {
        /* sstack_pop(interpreter->execution_stack, &next); */
        /* interpreter->program_pointer++; */
    }
    interpreter->program_pointer++;

    /* if (*interpreter->current_position != 0) { */
    /*     int curr = 0; */
    /*     sstack_peek(interpreter->execution_stack, &curr); */
    /*     interpreter->program_pointer = interpreter->parentheses_pointers_array[interpreter->current_parentheses_pos].open; */
    /* } else { */
    /*     sstack_pop(interpreter->execution_stack, &interpreter->current_parentheses_pos); */
    /* } */
    /* printf("handle_c_bracket: i: %d\tcurrent_parentheses_pos: %d\n", i, interpreter->current_parentheses_pos); */
    /* interpreter->program_pointer++; */
}

static int simple_bf_execute(unsigned char *program, simple_bf_interpreter_t *interpreter)
{
    char symbol;
    memset(interpreter->state_array, 0, sizeof(MAX_STATE_ARRAY_SIZE));
    interpreter->current_position = interpreter->state_array + (MAX_STATE_ARRAY_SIZE / 2);

    /* interpreter->execution_stack = sstack_new(100, sizeof(parentheses_pair)); */
    interpreter->execution_stack = sstack_new(100, sizeof(int));
    if (interpreter->execution_stack == NULL)
        return -1;

    while ((symbol = *interpreter->program_pointer) != '\0') {
        /* log_info("symbol: %c", symbol); */
        /* printf("symbol: %c -- position: %d -- ptr: %p\n", symbol, interpreter->program_pointer - program, interpreter->program_pointer); */
        /* sleep(1); */
        /* usleep(1000); */
        switch (symbol) {
            case '+':
                handle_plus(interpreter);
                break;
            case '-':
                handle_minus(interpreter);
                break;
            case '>':
                handle_greater_than(interpreter);
                break;
            case '<':
                handle_less_than(interpreter);
                break;
            case '.':
                handle_point(interpreter);
                break;
            case ',':
                handle_comma(interpreter);
                break;
            case '[':
                handle_o_bracket(interpreter);
                break;
            case ']':
                handle_c_bracket(interpreter);
                break;
            default:
                interpreter->program_pointer++;
        }
        /* log_info("current_position: %02x", *interpreter->current_position); */
        /* print_state(interpreter); */
        /* print_state_array(interpreter); */
    }

    free(interpreter->execution_stack);

    return 0;
}

static void simple_bf_interpreter_run(bf_interpreter_t *self, char *program, bf_error_t *error)
{
    /* printf("simple_bf_interpreter_run\n"); */

    int parse_error = 0;
    int execute_error = 0;

    error->error_code = 0;
    simple_bf_interpreter_t *inter = self->interpreter_p_data;
    inter->program_pointer = program;

    if (syntactic_check(program) != 0) {
        error->error_code = -1;

        return;
    }

    parse_error = simple_bf_parse(program, inter);
    if (parse_error != 0)
        error->error_code = parse_error;
    else
        inter->current_parentheses_pos = 0;

    inter->current_parentheses_pos = -1;
    execute_error = simple_bf_execute(program, inter);
    if (execute_error != 0)
        error->error_code = -1;
}

int simple_bf_interpreter_new(bf_interpreter_t *interpreter)
{
    int ret = 0;
    simple_bf_interpreter_t *priv;

    if ((priv = malloc(sizeof(simple_bf_interpreter_t))) == NULL) {
        ret = -1;
        goto exit_err;
    }

    /* priv->parentheses_pointers_stack = sstack_new(100, sizeof(parentheses_pair)); */
    /* if (priv->parentheses_pointers_stack == NULL) { */
    /*     ret = -1; */
    /*     goto exit_err_stack; */
    /* } */

    /* priv->parentheses_pointers_stack_out = sstack_new(1000, sizeof(parentheses_pair)); */
    /* if (priv->parentheses_pointers_stack_out == NULL) { */
    /*     ret = -1; */
    /*     goto exit_err_stack2; */
    /* } */

    interpreter->run = simple_bf_interpreter_run;
    interpreter->interpreter_p_data = priv;

    return 0;

exit_err_stack2:
    /* free(priv->parentheses_pointers_stack); */

exit_err_stack:
    free(priv);

exit_err:
    return ret;
}
