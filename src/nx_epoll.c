#include "nx_epoll.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

NxEpollReactor* nx_epoll_create(int max_events) {
    NxEpollReactor *reactor = (NxEpollReactor*)malloc(sizeof(NxEpollReactor));
    if (!reactor) return NULL;
    
    reactor->epoll_fd = epoll_create1(EPOLL_CLOEXEC);
    if (reactor->epoll_fd < 0) {
        free(reactor);
        return NULL;
    }
    
    reactor->max_events = max_events;
    reactor->events = (struct epoll_event*)malloc(sizeof(struct epoll_event) * max_events);
    if (!reactor->events) {
        close(reactor->epoll_fd);
        free(reactor);
        return NULL;
    }
    return reactor;
}

bool nx_epoll_add(NxEpollReactor *reactor, int fd, void *userdata) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET; // Edge Triggered ناهمزمان
    ev.data.ptr = userdata;
    return epoll_ctl(reactor->epoll_fd, EPOLL_CTL_ADD, fd, &ev) == 0;
}

int nx_epoll_wait_events(NxEpollReactor *reactor, int timeout_ms) {
    return epoll_wait(reactor->epoll_fd, reactor->events, reactor->max_events, timeout_ms);
}

void nx_epoll_destroy(NxEpollReactor *reactor) {
    if (reactor) {
        close(reactor->epoll_fd);
        free(reactor->events);
        free(reactor);
    }
}
