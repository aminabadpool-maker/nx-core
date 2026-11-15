#ifndef NX_MCP_H
#define NX_MCP_H

#include "nx_arena.h"
#include "nx_json.h"

typedef struct {
    int id;
    const char *method;
    char params_buf[256];
} NxMcpRequest;

int nx_mcp_parse_request(NxArena *arena, const char *raw_json, NxMcpRequest *out_req);

#endif
