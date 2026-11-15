#ifndef NX_EPOLL_H
#define NX_EPOLL_H

#include <sys/epoll.h>
#include <stdbool.h>

typedef struct {
    int epoll_fd;
    struct epoll_event *events;
    int max_events;
} NxEpollReactor;

NxEpollReactor* nx_epoll_create(int max_events);
bool nx_epoll_add(NxEpollReactor *reactor, int fd, void *userdata);
int nx_epoll_wait_events(NxEpollReactor *reactor, int timeout_ms);
void nx_epoll_destroy(NxEpollReactor *reactor);

#endif
