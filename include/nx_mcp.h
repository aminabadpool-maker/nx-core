#ifndef NX_MCP_H
#define NX_MCP_H

#include "nx_arena.h"

typedef struct {
    int id;
    const char *method;
    const char *params_json;
} NxMcpRequest;

int nx_mcp_parse_request(NxArena *arena, const char *raw_json, NxMcpRequest *out_req);

#endif
