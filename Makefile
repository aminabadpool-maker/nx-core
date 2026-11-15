CC = gcc
CFLAGS = -std=c11 -O3 -Wall -Wextra -pthread -Iinclude

all: nx_hub

nx_hub: src/nx_arena.c src/nx_reflect.c src/nx_json.c src/nx_mcp.c src/nx_sched.c src/nx_equity.c examples/nx_hub.c
	$(CC) $(CFLAGS) src/nx_arena.c src/nx_reflect.c src/nx_json.c src/nx_mcp.c src/nx_sched.c src/nx_equity.c examples/nx_hub.c -o nx_hub

clean:
	rm -f nx_hub test_arena
