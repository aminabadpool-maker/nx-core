CC = gcc
CFLAGS = -std=c11 -O3 -Wall -Wextra -pthread -Iinclude
SRC_DIR = src
INC_DIR = include
EX_DIR = examples
BENCH_DIR = bench

TARGETS = net_test kv_test net_bench stress_test arena_test integration_test

all: $(TARGETS)

net_test: $(EX_DIR)/net_test.c $(SRC_DIR)/nx_net.c
	$(CC) $(CFLAGS) $^ -o $@

kv_test: $(EX_DIR)/kv_test.c $(SRC_DIR)/nx_kv.c
	$(CC) $(CFLAGS) $^ -o $@

net_bench: $(BENCH_DIR)/net_bench.c
	$(CC) $(CFLAGS) $< -o $@

stress_test: $(BENCH_DIR)/stress_test.c
	$(CC) $(CFLAGS) $< -o $@

arena_test: $(EX_DIR)/arena_test.c $(SRC_DIR)/nx_arena.c
	$(CC) $(CFLAGS) $^ -o $@

integration_test: $(EX_DIR)/integration_test.c $(SRC_DIR)/nx_arena.c $(SRC_DIR)/nx_log.c $(SRC_DIR)/nx_ringbuf.c $(SRC_DIR)/nx_signal.c $(SRC_DIR)/nx_checksum.c $(SRC_DIR)/nx_threadpool.c $(SRC_DIR)/nx_hash.c $(SRC_DIR)/nx_strview.c $(SRC_DIR)/nx_proto.c $(SRC_DIR)/nx_metrics.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGETS) test_store.db stress_test.db *.o src/*.o

.PHONY: all clean
