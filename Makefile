CC = gcc
CFLAGS = -std=c11 -O3 -Wall -Wextra -pthread -Iinclude

all: nx_hub nx_bench test_neon test_log

nx_hub: src/nx_arena.c src/nx_reflect.c src/nx_json.c src/nx_mcp.c src/nx_sched.c src/nx_equity.c src/nx_log.c examples/nx_hub.c
	$(CC) $(CFLAGS) src/nx_arena.c src/nx_reflect.c src/nx_json.c src/nx_mcp.c src/nx_sched.c src/nx_equity.c src/nx_log.c examples/nx_hub.c -o nx_hub

nx_bench: src/nx_arena.c src/nx_sched.c src/nx_log.c bench/nx_bench.c
	$(CC) $(CFLAGS) src/nx_arena.c src/nx_sched.c src/nx_log.c bench/nx_bench.c -o nx_bench

test_neon: src/nx_neon.c tests/test_neon.c
	$(CC) $(CFLAGS) src/nx_neon.c tests/test_neon.c -o test_neon

test_log: src/nx_log.c tests/test_log.c
	$(CC) $(CFLAGS) src/nx_log.c tests/test_log.c -o test_log

clean:
	rm -f nx_hub nx_bench test_neon test_log test_arena
