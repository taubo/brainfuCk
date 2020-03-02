#ifndef BF_PARSER_H
#define BF_PARSER_H

#include "bf_sym.h"

/*
 * is it possible and/or practical to not be coupled to bf_sym?
 */
typedef struct bf_parser_input {
    void (*put)(struct bf_parser_input *self, bf_sym sym);
    void (*get)(struct bf_parser_input *self, bf_sym *sym);
} bf_parser_input;

typedef struct bf_parser {
    int (* parse)(struct bf_parser *self, bf_parser_input *input);
} bf_parser;

#define bf_parser_input_gen(a) \
typedef struct bf_parser_input { \
    void (*put)(struct bf_parser_input *self, #a sym); \
    void (*get)(struct bf_parser_input *self, #a *sym); \
} bf_parser_input; \

#endif
