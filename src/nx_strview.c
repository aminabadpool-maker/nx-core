#include "nx_strview.h"
#include <string.h>

NxStrView nx_strview_from(const char *str, size_t length) {
    NxStrView view = { .data = str, .length = length };
    return view;
}

bool nx_strview_equals(NxStrView a, NxStrView b) {
    if (a.length != b.length) return false;
    return memcmp(a.data, b.data, a.length) == 0;
}
