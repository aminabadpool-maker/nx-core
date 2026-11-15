#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "nx_proto.h"

int main() {
    printf("=== STARTING NX-CORE ZERO-COPY PROTOCOL TEST ===\n");

    // ساخت یک پکت تستی باینری در یک بافر ساده
    char buffer[256];
    NxProtocolHeader *h = (NxProtocolHeader*)buffer;
    h->magic = 0x4E580001; // 'NX' magic
    h->type = NX_MSG_COMMAND;
    h->payload_len = 13;

    const char *payload_data = "RUN_SYSTEM_AI";
    memcpy(buffer + sizeof(NxProtocolHeader), payload_data, 13);

    size_t total_packet_size = sizeof(NxProtocolHeader) + 13;

    // پارس کردن پکت بدون ایجاد حتی یک کپی حافظه یا تخصیص در هپ
    NxProtocolHeader parsed_header;
    NxSliceView payload_view;

    bool success = nx_proto_parse_zerocopy(buffer, total_packet_size, &parsed_header, &payload_view);
    
    assert(success);
    printf("[PROTO] Magic: 0x%X\n", parsed_header.magic);
    printf("[PROTO] Type: 0x%X\n", parsed_header.type);
    printf("[PROTO] Payload Length: %u\n", parsed_header.payload_len);
    
    // چاپ مستقیم داده از روی پوینتر بدون کپی شدن رشته
    printf("[PROTO] Zero-Copy Payload View: %.*s\n", (int)payload_view.len, payload_view.ptr);
    
    assert(payload_view.len == 13);
    assert(memcmp(payload_view.ptr, "RUN_SYSTEM_AI", 13) == 0);

    printf("=== ZERO-COPY PROTOCOL TEST PASSED SUCCESSFULLY! ===\n");
    return 0;
}
