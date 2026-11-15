#ifndef NX_SIGNAL_H
#define NX_SIGNAL_H

typedef void (*nx_shutdown_callback_t)(void);

void nx_signal_init(nx_shutdown_callback_t cb);
int nx_signal_is_received(void);

#endif // NX_SIGNAL_H
