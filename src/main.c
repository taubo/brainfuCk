#include <stdio.h>
#include "bf_sym.h"

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

bf_parser_input_gen(bf_sym);

int main(int argc, char **argv)
{
    char *input_filename = argv[1];
    FILE *input_file = fopen(input_filename, "r");
    char bf_character;

    while ((bf_character = fgetc(input_file)) != EOF) {
        printf("bf_character: %c\n", bf_character);
        printf("show: %s\n", bf_sym_show(get_bf_sym(bf_character)));
    }

    return 0;
}
