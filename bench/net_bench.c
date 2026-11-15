#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define ITERATIONS 500

long get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000000L + ts.tv_nsec;
}

int main() {
    printf("[*] Starting nx_net micro-benchmark (%d connections)...\n", ITERATIONS);
    
    long total_time = 0;
    int success_count = 0;

    for (int i = 0; i < ITERATIONS; i++) {
        long start = get_time_ns();

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) continue;

        struct sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(8080);
        inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

        if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == 0) {
            char buffer[256];
            read(sock, buffer, sizeof(buffer) - 1);
            success_count++;
        }

        close(sock);
        long end = get_time_ns();
        total_time += (end - start);
    }

    printf("[RESULTS] Success: %d/%d connections\n", success_count, ITERATIONS);
    printf("[RESULTS] Average latency per connection: %ld microseconds\n", (total_time / ITERATIONS) / 1000);
    return 0;
}
