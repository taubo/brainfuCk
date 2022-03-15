#include <stdlib.h>
#include <stdio.h>
#include "brainfuCk.h"

/* interpreters */
#include "simple_bf_interpreter.h"

unsigned long get_file_size(FILE *file)
{
    fseek(file, 0L, SEEK_END);
    unsigned long size = ftell(file);
    rewind(file);
    return size;
}

void read_program(char *dst, FILE *program_filename)
{
    char character;
    int i = 0;

    while ((character = fgetc(program_filename)) != EOF) {
        dst[i++] = character;
    }
}

int main(int argc, char **argv)
{
    /* TODO: argument parsing */
    char *input_filename;

    if (argc == 2) {
        input_filename = argv[1];
    } else {
        puts("Please, specify a bf file");

        return -1;
    }
    FILE *input_file = fopen(input_filename, "r");

    char *input_program;
    unsigned long file_size = get_file_size(input_file);

    input_program = malloc(file_size);
    read_program(input_program, input_file);

    bf_interpreter_t interpreter;
    simple_bf_interpreter_new(&interpreter);

    interpreter.run(&interpreter, input_program);

    free(input_program);
    fclose(input_file);

    return 0;
}
