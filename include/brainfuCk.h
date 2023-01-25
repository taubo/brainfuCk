#ifndef BRAINFUCK_H
#define BRAINFUCK_H

typedef struct {
    int error_code;
} bf_error_t ;

typedef struct bf_interpreter_t {
    void *interpreter_p_data;
    void (* run)(struct bf_interpreter_t *self, char *program, bf_error_t *error);
} bf_interpreter_t;

#endif
