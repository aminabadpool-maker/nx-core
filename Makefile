CC = gcc
CFLAGS = -std=c11 -O3 -Wall -Wextra -pthread -Iinclude

SRC_DIR = src
INC_DIR = include
EX_DIR = examples
BENCH_DIR = bench

# نام فایل‌های اجرایی نهایی
TARGETS = net_test kv_test net_bench

all: $(TARGETS)

net_test: $(EX_DIR)/net_test.c $(SRC_DIR)/nx_net.c
	$(CC) $(CFLAGS) $^ -o $@

kv_test: $(EX_DIR)/kv_test.c $(SRC_DIR)/nx_kv.c
	$(CC) $(CFLAGS) $^ -o $@

net_bench: $(BENCH_DIR)/net_bench.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGETS) test_store.db *.o

.PHONY: all clean
