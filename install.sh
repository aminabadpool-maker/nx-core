#!/bin/bash
gcc -std=c11 -O3 -pthread -Iinclude src/*.c examples/nx_hub.c -o nx_hub && echo "Built successfully!"
