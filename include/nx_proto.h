#ifndef NX_PROTO_H
#define NX_PROTO_H

#include <stdint.h>
#include <stddef.h>

#pragma pack(push, 1)
typedef struct {
    uint32_t magic;
    uint32_t msg_type;
    uint32_t payload_len;
    uint32_t checksum;
} NxHeader;
#pragma pack(pop)

size_t nx_proto_serialize(NxHeader *hdr, const char *payload, char *out_buf, size_t buf_size);

#endif // NX_PROTO_H
