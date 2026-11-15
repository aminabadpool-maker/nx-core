#include "nx_mcp.h"
#include <string.h>
#include <stdlib.h>

int nx_mcp_parse_request(NxArena *arena, const char *raw_json, NxMcpRequest *out_req) {
    if (!raw_json || !out_req) return -1;

    char id_str[32];
    if (nx_json_parse_field(arena, raw_json, "id", id_str, sizeof(id_str)) == 0) {
        out_req->id = atoi(id_str);
    } else {
        out_req->id = 0;
    }

    char method_str[128];
    if (nx_json_parse_field(arena, raw_json, "method", method_str, sizeof(method_str)) == 0) {
        out_req->method = nx_arena_alloc(arena, strlen(method_str) + 1);
        strcpy((char*)out_req->method, method_str);
    } else {
        out_req->method = "unknown";
    }

    strcpy(out_req->params_buf, "Parsed via nx_core zero-copy json engine");
    return 0;
}
