#include "nx_net.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

struct NxNetServer {
    int server_fd;
    int port;
};

NxNetServer* nx_net_server_init(int port) {
    NxNetServer *server = (NxNetServer*)malloc(sizeof(NxNetServer));
    if (!server) return NULL;

    server->port = port;
    server->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->server_fd < 0) {
        free(server);
        return NULL;
    }

    int opt = 1;
    setsockopt(server->server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server->server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        close(server->server_fd);
        free(server);
        return NULL;
    }

    if (listen(server->server_fd, 16) < 0) {
        close(server->server_fd);
        free(server);
        return NULL;
    }

    return server;
}

bool nx_net_handle_connection(NxNetServer *server, char *buffer, size_t max_len) {
    if (!server) return false;

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    int client_fd = accept(server->server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) return false;

    ssize_t bytes_read = read(client_fd, buffer, max_len - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        
        // پاسخ ساده HTTP/TCP به کلاینت
        const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello from nx-core Agent Network!\n";
        write(client_fd, response, strlen(response));
    }

    close(client_fd);
    return true;
}

void nx_net_server_destroy(NxNetServer *server) {
    if (!server) return;
    close(server->server_fd);
    free(server);
}
