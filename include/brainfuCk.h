#ifndef BRAINFUCK_H
#define BRAINFUCK_H

typedef struct bf_interpreter_t {
    void *interpreter_p_data;
    void (* run)(struct bf_interpreter_t *self, char *program);
} bf_interpreter_t;

#endif
