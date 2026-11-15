CC = gcc
CFLAGS = -std=c11 -O3 -Wall -Wextra -Iinclude

all: test_arena

test_arena: src/nx_arena.c src/nx_reflect.c src/nx_mcp.c tests/test_arena.c
	$(CC) $(CFLAGS) src/nx_arena.c src/nx_reflect.c src/nx_mcp.c tests/test_arena.c -o test_arena

clean:
	rm -f test_arena
