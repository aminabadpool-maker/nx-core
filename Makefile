CC = gcc
# اضافه شدن فلگ‌های امنیتی و AddressSanitizer برای شکار هرگونه باگ حافظه زیر تست سنگین
CFLAGS = -std=c11 -O3 -Wall -Wextra -Werror -pthread -fsanitize=address,undefined -Iinclude
SRC_DIR = src
INC_DIR = include
EX_DIR = examples
BENCH_DIR = bench

TARGETS = net_test kv_test net_bench stress_test arena_test integration_test llm_agent_test brutal_stress advanced_test

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

llm_agent_test: $(EX_DIR)/llm_agent_test.c $(SRC_DIR)/nx_arena.c $(SRC_DIR)/nx_log.c $(SRC_DIR)/nx_threadpool.c $(SRC_DIR)/nx_metrics.c $(SRC_DIR)/nx_checksum.c $(SRC_DIR)/nx_proto.c
	$(CC) $(CFLAGS) $^ -o $@

brutal_stress: $(BENCH_DIR)/brutal_stress.c $(SRC_DIR)/nx_arena.c $(SRC_DIR)/nx_threadpool.c $(SRC_DIR)/nx_metrics.c
	$(CC) $(CFLAGS) $^ -o $@

advanced_test: $(EX_DIR)/advanced_test.c $(SRC_DIR)/nx_lockfree.c $(SRC_DIR)/nx_simd.c $(SRC_DIR)/nx_mmap_db.c $(SRC_DIR)/nx_epoll.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGETS) test_store.db stress_test.db test_mmap.db *.o src/*.o

.PHONY: all clean
