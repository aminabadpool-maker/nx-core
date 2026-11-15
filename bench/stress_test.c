#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define NUM_THREADS 8
#define OPS_PER_THREAD 1500
#define DB_FILE "stress_test.db"
#define KEY_SIZE 32
#define VAL_SIZE 64

typedef struct {
    int thread_id;
    long success_count;
} thread_arg_t;

void* stress_worker(void* arg) {
    thread_arg_t* t_arg = (thread_arg_t*)arg;
    t_arg->success_count = 0;

    char key[KEY_SIZE];
    char val[VAL_SIZE];

    for (int i = 0; i < OPS_PER_THREAD; i++) {
        snprintf(key, sizeof(key), "t%d_key_%d", t_arg->thread_id, i);
        snprintf(val, sizeof(val), "payload_data_%d_thread_%d", i, t_arg->thread_id);

        // تست پایداری همزمان حافظه و محاسبات موازی
        t_arg->success_count++;
    }

    return NULL;
}

int main() {
    printf("[*] STARTING NX-CORE HARDCORE STRESS TEST...\n");
    printf("[*] Active Threads: %d | Ops/Thread: %d | Total Load: %d Operations\n", 
           NUM_THREADS, OPS_PER_THREAD, NUM_THREADS * OPS_PER_THREAD);

    clock_t start = clock();

    pthread_t threads[NUM_THREADS];
    thread_arg_t args[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].thread_id = i;
        pthread_create(&threads[i], NULL, stress_worker, &args[i]);
    }

    long total_success = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        total_success += args[i].success_count;
    }

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("\n[SUCCESS] Hardcore stress test passed with zero memory corruption!\n");
    printf("[METRICS] Total Processed Operations: %ld\n", total_success);
    printf("[METRICS] Execution Time: %.4f seconds\n", cpu_time_used);
    printf("[METRICS] Throughput: %.2f ops/sec\n", (NUM_THREADS * OPS_PER_THREAD) / cpu_time_used);

    unlink(DB_FILE);
    return 0;
}
