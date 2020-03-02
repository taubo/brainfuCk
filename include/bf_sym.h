#ifndef BF_SYM_H
#define BF_SYM_H

typedef enum {
    PLUS,
    MINUS,
    INCR_PTR,
    DECR_PTR,
    INPUT,
    OUTPUT,
    JMP_BACKWARD,
    JMP_FORWARD,
    COMMENT,
} bf_sym;

#endif
