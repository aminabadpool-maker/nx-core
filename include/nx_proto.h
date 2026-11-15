#ifndef NX_PROTO_H
#define NX_PROTO_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// نمای برشی پوینتر برای صفرکپی
typedef struct {
    const char *ptr;
    size_t len;
} NxSliceView;

typedef enum {
    NX_MSG_PING = 0x01,
    NX_MSG_COMMAND = 0x02,
    NX_MSG_METRICS = 0x03,
    NX_MSG_UNKNOWN = 0xFF
} NxMsgType;

// ساختار هدر پکت (بدون تراز اجباری ۶۴ بایتی روی پکت‌های خام شبکه برای جلوگیری از خطای UBSan)
typedef struct {
    uint32_t magic;
    uint8_t type;
    uint32_t payload_len;
} __attribute__((packed)) NxProtocolHeader;

// پارسر صفرکپی ایمن در برابر عدم تراز حافظه
bool nx_proto_parse_zerocopy(const char *raw_data, size_t total_len, NxProtocolHeader *out_header, NxSliceView *out_payload);

#endif
