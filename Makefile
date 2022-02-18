TARGET=brainfuCk

SRC_DIR=src
INCLUDE_DIR=include
SRCS=$(wildcard $(SRC_DIR)/*.c)
INCLUDES=$(wildcard $(INCLUDE_DIR)/*.h)

SRCS_BF=src/main.c

$(TARGET): $(SRCS_BF)
	@mkdir -p bin
	$(CC) -DDEBUG -g $(SRCS_BF) -I$(INCLUDE_DIR) -o bin/$@-debug
	$(CC) -O3 $(SRCS_BF) -I$(INCLUDE_DIR) -o bin/$@

par_matching_test: src/par_matching.c src/simple_stack.c
	$(CC) $^ -I$(INCLUDE_DIR) -o bin/$@

SRCS_STACK=src/simple_stack.c src/simple_stack_app.c

simple_stack_app: $(SRCS_STACK)
	$(CC) $(SRCS_STACK) -g -Wall -I$(INCLUDE_DIR) -o bin/$@

clean:
	rm -rf bin/*

.PHONY: clean run
