#include "nx_net.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>

static volatile bool keep_running = true;

void handle_sigint(int sig) {
    (void)sig;
    keep_running = false;
    printf("\n[*] Shutting down nx-core network daemon gracefully...\n");
}

int main() {
    signal(SIGINT, handle_sigint);

    printf("[*] Starting permanent nx_net daemon server on port 8080...\n");
    NxNetServer *server = nx_net_server_init(8080);
    if (!server) {
        fprintf(stderr, "[ERROR] Failed to initialize network server.\n");
        return 1;
    }

    printf("[OK] Server is running continuously. Press Ctrl+C to stop.\n");
    
    char buffer[1024];
    while (keep_running) {
        // پردازش مداوم درخواست‌ها بدون خاموش شدن
        if (nx_net_handle_connection(server, buffer, sizeof(buffer))) {
            printf("[REQUEST PROCESSED] Client connected and served successfully.\n");
        }
    }

    nx_net_server_destroy(server);
    printf("[OK] Server stopped cleanly.\n");
    return 0;
}
