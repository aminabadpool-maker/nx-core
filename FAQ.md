# Frequently Asked Questions (FAQ)

### 1. Why build an AI agent core/framework in pure C? Isn't Python the standard?
Python is great for high-level scripting, but when it comes to raw execution speed, predictability, minimal memory footprint, and absolute system-level control, pure C (C11) with -O3 optimization remains unmatched. nx-core proves that you don't need heavy runtimes or massive frameworks to build a high-performance agent architecture.

### 2. What are the external dependencies of nx-core?
Zero dependencies. The project is intentionally designed to rely solely on standard POSIX system calls and standard C libraries. No bloated third-party packages, no complex build chains—just clean, portable systems programming.

### 3. How does nx_kv store data without a traditional database like SQLite?
nx_kv uses Memory-Mapped Files (mmap). Instead of traditional read/write overhead, it maps the database file directly into the process's address space. This allows blazing-fast lookups and persistent storage managed natively by the operating system kernel with zero garbage collection lag.

### 4. How can I build and run the project?
Everything is automated through a clean, native Makefile. Simply run `make` and `./kv_test`. To clean up, run `make clean`.
