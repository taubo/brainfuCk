#include <stdbool.h>
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
int sub_cnt = 0;
typedef struct {
    char *open_bracket[20];
    int cnt[20];
} loop;

void print_out_tape(simple_bf_interpreter *interpreter)
{
    for (int i = 0; i < 50; ++i) {
        if (i == interpreter->curr_pos)
            printf(" [%d] ", interpreter->bf_output_tape[i]);
        else
            printf(" %d ", interpreter->bf_output_tape[i]);
    }
    printf("\n");
}

void tape_print_untill(char *tape_char, int untill)
{
    for (int i = 0; i < untill; ++i) {
        if (i != untill) {
            printf("%c", tape_char[i]);
        } else {
            printf("[%c]", tape_char[i]);
        }
    }
    printf("\n");
}

bool execute = true;

int simple_bf_interpreter_run(simple_bf_interpreter *self, bf_tape *program_tape)
{
    int _size = program_tape->size;
    int cnt = 0;
    loop _loop;

    dbg("_size: %d\n", _size);
    while (cnt <= _size) {
        dbg("cnt: %d\n", cnt);
        dbg("bracket_cnt: %d\n", bracket_cnt);
        // dbg("tape char: %c => ", program_tape->tape[cnt]);
        if (execute) {
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
                default:
                    break;
            }
        }
        switch (program_tape->tape[cnt]) {
            case '[':
                {
                    _loop.open_bracket[bracket_cnt] = &program_tape->tape[cnt + 1];
                    _loop.cnt[bracket_cnt] = cnt;
                    // printf("%c\n", *(_loop.open_bracket[bracket_cnt]));
                    bracket_cnt++;
                    if (!execute)
                        sub_cnt++;
                    if (self->bf_output_tape[self->curr_pos] == 0) {
                        // jmp forward
                        // dbg("cnt: %d\n", cnt);
                        // dbg("_size: %d\n", _size);
                        execute = false;
                    }
                }
                break;
            case ']':
                if (sub_cnt == 0)
                    execute = true;
                if (!execute)
                    sub_cnt--;
                if (self->bf_output_tape[self->curr_pos] != 0) {
                    // jmp backward
                    if (bracket_cnt > 0)
                        cnt = _loop.cnt[bracket_cnt - 1];
                    dbg("cnt: %d\n", cnt);
                } else {
                    if (bracket_cnt > 0) {
                        bracket_cnt--;
                    }
                    /*
                       else {
                       return -2;
                       }
                       */
                }
                break;
            default:
                break;
        }
        cnt++;
#ifdef DEBUG
        tape_print_untill(program_tape->tape, cnt);
#endif
#ifdef DEBUG
        print_out_tape(self);
#endif
    }

    // if (_loop.cnt > 0)
    //     return -3;
    //
    return 0;
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
    // printf("ret: %d\n", simple_bf_interpreter_run(&interpreter, &input_tape));
    simple_bf_interpreter_run(&interpreter, &input_tape);

    return 0;
}
