#include "nx_arena.h"
#include "nx_json.h"
#include "nx_mcp.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

int main() {
    printf("--- Testing NxCore JSON & MCP Modules ---\n");

    NxArena *arena = nx_arena_create(2048);
    assert(arena != NULL);

    // تست پارسر JSON
    const char *sample_json = "{\"jsonrpc\":\"2.0\",\"id\":42,\"method\":\"tools/call\"}";
    char method_result[64];
    int res = nx_json_parse_field(arena, sample_json, "method", method_result, sizeof(method_result));
    assert(res == 0);
    assert(strcmp(method_result, "tools/call") == 0);
    printf("[OK] JSON field parsing passed (method = %s).\n", method_result);

    // تست پروتکل MCP
    NxMcpRequest mcp_req;
    int mcp_res = nx_mcp_parse_request(arena, sample_json, &mcp_req);
    assert(mcp_res == 0);
    assert(mcp_req.id == 42);
    assert(strcmp(mcp_req.method, "tools/call") == 0);
    printf("[OK] MCP Request parsing passed (ID = %d, Method = %s).\n", mcp_req.id, mcp_req.method);

    nx_arena_free(arena);
    printf("--- All JSON & MCP Tests Passed Successfully! ---\n");
    return 0;
}
