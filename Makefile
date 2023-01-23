TARGET=brainfuCk

SRC_DIR=src
INCLUDE_DIR=include
OBJ_DIR=build
SRCS=$(wildcard $(SRC_DIR)/*.c)
INCLUDES=$(wildcard $(INCLUDE_DIR)/*.h)

SRCS_BF=src/main.c
SRCS_BF_NEW=src/brainfuCk.c

OBJS=$(subst $(SRC_DIR),$(OBJ_DIR),$(SRCS:%.c=%.o))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $^ -o $@ -I$(INCLUDE_DIR)

$(TARGET): prepare $(OBJS)
	$(CC) -DDEBUG -g $(OBJS) -I$(INCLUDE_DIR) -o bin/$@-debug
	$(CC) -O3 $(OBJS) -I$(INCLUDE_DIR) -o bin/$@

prepare:
	mkdir -p bin
	mkdir -p build

par_matching_test: src/par_matching.c src/simple_stack.c
	$(CC) $^ -I$(INCLUDE_DIR) -o bin/$@

SRCS_STACK=src/simple_stack.c src/simple_stack_app.c

simple_stack_app: $(SRCS_STACK)
	$(CC) $(SRCS_STACK) -g -Wall -I$(INCLUDE_DIR) -o bin/$@

clean:
	rm -rf bin/*
	rm -rf build/*

.PHONY: clean run
