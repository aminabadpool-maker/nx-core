#include "nx_proto.h"
#include "nx_checksum.h"
#include <string.h>

size_t nx_proto_serialize(NxHeader *hdr, const char *payload, char *out_buf, size_t buf_size) {
    size_t total_size = sizeof(NxHeader) + hdr->payload_len;
    if (buf_size < total_size) return 0;

    hdr->checksum = nx_checksum_crc32(payload, hdr->payload_len);
    memcpy(out_buf, hdr, sizeof(NxHeader));
    memcpy(out_buf + sizeof(NxHeader), payload, hdr->payload_len);
    return total_size;
}
