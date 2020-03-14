TARGET=brainfuCk

SRC_DIR=src
INCLUDE_DIR=include
SRCS=$(wildcard $(SRC_DIR)/*.c)
INCLUDES=$(wildcard $(INCLUDE_DIR)/*.h)

$(TARGET): $(SRCS)
	@mkdir -p bin
	$(CC) -DDEBUG -g $(SRCS) -I$(INCLUDE_DIR) -o bin/$@-debug
	$(CC) -O3 $(SRCS) -I$(INCLUDE_DIR) -o bin/$@

clean:
	rm -rf bin/*

.PHONY: clean run
