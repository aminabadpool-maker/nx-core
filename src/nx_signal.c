#include "nx_signal.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

static volatile int shutdown_flag = 0;
static nx_shutdown_callback_t user_callback = NULL;

static void handle_signal(int sig) {
    (void)sig;
    shutdown_flag = 1;
    if (user_callback) {
        user_callback();
    }
}

void nx_signal_init(nx_shutdown_callback_t cb) {
    user_callback = cb;
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
}

int nx_signal_is_received(void) {
    return shutdown_flag;
}
