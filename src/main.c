#include <stdio.h>
#include "bf_sym.h"
#include "bf_parser.h"

/*
 * what if i did something like this:
 * something[] = {
 * { "+", PLUS },
 * { "-", MINUS },
 * { ">", INCR_PTR },
 * { "<", DECR_PTR },
 * ....
 * }
 */
bf_sym get_bf_sym(char character)
{
    switch (character) {
        case '+':
            return PLUS;
        case '-':
            return MINUS;
        case '>':
            return INCR_PTR;
        case '<':
            return DECR_PTR;
        case '.':
            return INPUT;
        case ',':
            return OUTPUT;
        case '[':
            return JMP_BACKWARD;
        case ']':
            return JMP_FORWARD;
        default:
            return COMMENT;
    };
}

/*
 * same as above
 */
char *bf_sym_show(bf_sym sym)
{
    switch (sym) {
        case PLUS:
            return "PLUS";
        case MINUS:
            return "MINUS";
        case INCR_PTR:
            return "INCR_PTR";
        case DECR_PTR:
            return "DECR_PTR";
        case INPUT:
            return "INPUT";
        case OUTPUT:
            return "OUTPUT";
        case JMP_BACKWARD:
            return "JMP_BACKWARD";
        case JMP_FORWARD:
            return "JMP_FORWARD";
        default:
            return "COMMENT";
    }
}

#define MAX_NESTED_LOOP_COUNTER 10
#define BF_TAPE_SIZE            65535

typedef struct simple_bf_interpreter {
    unsigned curr_pos;
    unsigned simple_bracket_stack[MAX_NESTED_LOOP_COUNTER];
    unsigned char bf_output_tape[BF_TAPE_SIZE]; 
} simple_bf_interpreter;

void bf_incr_data(simple_bf_interpreter *interpreter)
{
    interpreter->bf_output_tape[interpreter->curr_pos]++;
}

void bf_decr_data(simple_bf_interpreter *interpreter)
{
    interpreter->bf_output_tape[interpreter->curr_pos]--;
}

void bf_incr_ptr(simple_bf_interpreter *interpreter)
{
    if (interpreter->curr_pos < BF_TAPE_SIZE)
        interpreter->curr_pos++;
}

void bf_decr_ptr(simple_bf_interpreter *interpreter)
{
    if (interpreter->curr_pos > 0)
        interpreter->curr_pos--;
}

void bf_input(simple_bf_interpreter *interpreter)
{
    interpreter->bf_output_tape[interpreter->curr_pos] = getchar();
}

void bf_output(simple_bf_interpreter *interpreter)
{
    putchar(interpreter->bf_output_tape[interpreter->curr_pos]);
}

#ifdef DEBUG
#define dbg(...) printf("DEBUG: "__VA_ARGS__)
#else
#define dbg(...)
#endif

int simple_bf_interpreter_run(simple_bf_interpreter *self, char character)
{
    dbg("curr_pos: %u\n", self->curr_pos);
    switch (character) {
        case '+':
            bf_incr_data(self);
            break;
        case '-':
            bf_decr_data(self);
            break;
        case '>':
            bf_incr_ptr(self);
            break;
        case '<':
            bf_decr_ptr(self);
            break;
        case '.':
            bf_input(self);
            break;
        case ',':
            bf_output(self);
            break;
        case '[':
            break;
        case ']':
            break;
        default:
            break;
    }
}

int main(int argc, char **argv)
{
    char *input_filename = argv[1];
    FILE *input_file = fopen(input_filename, "r");
    char bf_character;

    /*
     * TODO: new()
     */
    simple_bf_interpreter interpreter = {
        .curr_pos = 0,
    };

    while ((bf_character = fgetc(input_file)) != EOF) {
        // printf("bf_character: %c\n", bf_character);
        // printf("show: %s\n", bf_sym_show(get_bf_sym(bf_character)));
        simple_bf_interpreter_run(&interpreter, bf_character);
    }

    return 0;
}
