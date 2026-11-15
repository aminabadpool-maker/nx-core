#include "nx_net.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("[*] Starting nx_net test server on port 8080...\n");
    NxNetServer *server = nx_net_server_init(8080);
    if (!server) {
        fprintf(stderr, "[ERROR] Failed to initialize network server.\n");
        return 1;
    }

    printf("[OK] Server is running. Send a request (e.g., curl http://127.0.0.1:8080)\n");
    
    char buffer[1024];
    // تست پذیرش یک اتصال ورودی برای نمونه
    if (nx_net_handle_connection(server, buffer, sizeof(buffer))) {
        printf("[RECEIVED] Data from client:\n%s\n", buffer);
    }

    nx_net_server_destroy(server);
    printf("[OK] Server shut down cleanly.\n");
    return 0;
}
