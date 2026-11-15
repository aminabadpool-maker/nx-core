#include "nx_proto.h"
#include <string.h>

#define NX_LIKELY(x)   __builtin_expect(!!(x), 1)
#define NX_UNLIKELY(x) __builtin_expect(!!(x), 0)

bool nx_proto_parse_zerocopy(const char *raw_data, size_t total_len, NxProtocolHeader *out_header, NxSliceView *out_payload) {
    if (NX_UNLIKELY(!raw_data || total_len < sizeof(NxProtocolHeader))) {
        return false;
    }

    // کپی امن فیلدها برای جلوگیری قطعی از خطای Misaligned Access در UBSan
    NxProtocolHeader temp_header;
    memcpy(&temp_header, raw_data, sizeof(NxProtocolHeader));

    if (NX_UNLIKELY(temp_header.magic != 0x4E580001)) {
        return false;
    }

    if (NX_UNLIKELY(sizeof(NxProtocolHeader) + temp_header.payload_len > total_len)) {
        return false;
    }

    out_header->magic = temp_header.magic;
    out_header->type = temp_header.type;
    out_header->payload_len = temp_header.payload_len;

    // Zero-Copy Slice روی بافر ورودی
    out_payload->ptr = raw_data + sizeof(NxProtocolHeader);
    out_payload->len = temp_header.payload_len;

    return true;
}
