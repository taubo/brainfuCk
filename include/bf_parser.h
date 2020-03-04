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
    int (* parse)(struct bf_parser *self);
} bf_parser;

int bf_parser_put(bf_parser *self, bf_sym sym);
int bf_parser_parse(bf_parser *self);

#endif
