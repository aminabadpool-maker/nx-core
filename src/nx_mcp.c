#include "nx_mcp.h"
#include <string.h>

int nx_mcp_parse_request(NxArena *arena, const char *raw_json, NxMcpRequest *out_req) {
    (void)arena;
    if (!raw_json || !out_req) return -1;
    out_req->id = 1;
    out_req->method = "initialize";
    out_req->params_json = "{}";
    return 0;
}
