#include "nx_json.h"
#include <string.h>
#include <stdio.h>

int nx_json_parse_field(NxArena *arena, const char *json_str, const char *key, char *out_val, size_t max_len) {
    (void)arena;
    if (!json_str || !key || !out_val) return -1;

    char search_pattern[128];
    snprintf(search_pattern, sizeof(search_pattern), "\"%s\"", key);

    char *pos = strstr((char*)json_str, search_pattern);
    if (!pos) return -1;

    pos = strchr(pos, ':');
    if (!pos) return -1;
    pos++;

    while (*pos == ' ' || *pos == '\t' || *pos == '\"') pos++;

    size_t i = 0;
    while (*pos && *pos != '\"' && *pos != ',' && *pos != '}' && i < max_len - 1) {
        out_val[i++] = *pos++;
    }
    out_val[i] = '\0';

    return 0;
}
