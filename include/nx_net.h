#ifndef NX_NET_H
#define NX_NET_H

#include <stdbool.h>
#include <stddef.h>

typedef struct NxNetServer NxNetServer;

// راه‌اندازی سرور TCP روی پورت مشخص
NxNetServer* nx_net_server_init(int port);

// پذیرش اتصال ورودی و خواندن داده‌ها با استفاده از آرنا
bool nx_net_handle_connection(NxNetServer *server, char *buffer, size_t max_len);

// بستن و آزادسازی سرور
void nx_net_server_destroy(NxNetServer *server);

#endif
