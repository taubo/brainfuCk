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

/* typedef struct {
 *     unsigned size;
 *     unsigned stack_pointer;
 *     struct {
 *         unsigned positions[MAX_NESTED_LOOP_COUNTER];
 *     } bracket_info;
 * } bf_jump; */

typedef struct simple_bf_interpreter {
    unsigned curr_pos;
    unsigned simple_bracket_stack[MAX_NESTED_LOOP_COUNTER];
    unsigned char bf_output_tape[BF_TAPE_SIZE];
    unsigned char bf_input_tape[BF_TAPE_SIZE];
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

int simple_bf_interpreter_put(simple_bf_interpreter *self, char character)
{

}

typedef struct {
    char *tape;
    int size;
} bf_tape;

int bracket_cnt = 0;
typedef struct {
    char *open_bracket[10];
    int cnt;
} loop;

void print_out_tape(simple_bf_interpreter *interpreter)
{
    for (int i = 0; i < 6; ++i) {
        if (i == interpreter->curr_pos)
            printf("[ %d ] ", interpreter->bf_output_tape[i]);
        else
            printf(" %d ", interpreter->bf_output_tape[i]);
    }
    printf("\n");
}

int simple_bf_interpreter_run(simple_bf_interpreter *self, bf_tape *program_tape)
{
    int _size = program_tape->size;
    int cnt = 0;
    loop _loop;

    while (cnt != _size) {
        dbg("tape char: %c => ", program_tape->tape[cnt]);
        switch (program_tape->tape[cnt]) {
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
                bf_output(self);
                break;
            case ',':
                bf_input(self);
                break;
            case '[':
                {
                    _loop.open_bracket[bracket_cnt] = &program_tape->tape[cnt + 1];
                    _loop.cnt = cnt;
                    // printf("%c\n", *(_loop.open_bracket[bracket_cnt]));
                    bracket_cnt++;
                    if (self->bf_output_tape[self->curr_pos] != 0) {
                        // continue;
                    } else {
                        // jmp forward
                        dbg("cnt: %d\n", cnt);
                        dbg("_size: %d\n", _size);
                        while (cnt < _size) {
                            dbg("next: %c\n", program_tape->tape[cnt + 1]);
                            if (program_tape->tape[cnt + 1] != ']')
                                cnt++;
                            else
                                break;
                        }
                        if (cnt == _size)
                            return -1;
                    }
                }
                break;
            case ']':
                if (self->bf_output_tape[self->curr_pos] != 0) {
                    // jmp backward
                    cnt = _loop.cnt;
                } else {
                    if (bracket_cnt > 0) {
                        bracket_cnt--;
                    } else {
                        return -1;
                    }
                }
                break;
            default:
                break;
        }
        cnt++;
#ifdef DEBUG
        print_out_tape(self);
#endif
    }


    if (_loop.cnt > 0)
        return -1;
}

#define MAX_BF_PROGRAM  65536

char bf_program_tape_chars[MAX_BF_PROGRAM];

int main(int argc, char **argv)
{
    char *input_filename = argv[1];
    FILE *input_file = fopen(input_filename, "r");
    char bf_character;

    bf_tape input_tape = {
        .size = 0,
        .tape = bf_program_tape_chars,
    };

    /*
     * TODO: new()
     */
    simple_bf_interpreter interpreter = {
        .curr_pos = 0,
    };

    while ((bf_character = fgetc(input_file)) != EOF) {
        // printf("bf_character: %c\n", bf_character);
        // printf("show: %s\n", bf_sym_show(get_bf_sym(bf_character)));
        // simple_bf_interpreter_put(&interpreter, bf_character);
        /*
         * TODO: strip space, tab, end of line ....
         */
        input_tape.size++;
        input_tape.tape[input_tape.size] = bf_character;
    }
    simple_bf_interpreter_run(&interpreter, &input_tape);

    return 0;
}
