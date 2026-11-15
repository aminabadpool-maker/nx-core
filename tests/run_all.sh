#!/bin/bash
echo "=== Running nx-core Full Test Suite ==="

# کامپایل و اجرای تست‌ها
make clean && make all

# اجرای تست‌ها
./test_neon
./test_log
./nx_hub
./nx_bench

echo "=== All Tests Passed Successfully! ==="
