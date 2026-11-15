#include <stdio.h>
#include <string.h>
#include "nx_arena.h"

int main() {
    printf("[*] Initializing NX-Core Arena Allocator...\n");
    
    // ساخت یک آرنای ۱ مگابایتی
    NxArena *arena = nx_arena_create(1024 * 1024);
    if (!arena) {
        fprintf(stderr, "[-] Failed to create arena!\n");
        return 1;
    }
    printf("[+] Arena created successfully (Capacity: 1MB via mmap)\n");

    // تخصیص حافظه سریع و خطی
    int *numbers = (int*)nx_arena_alloc(arena, 100 * sizeof(int));
    char *str = (char*)nx_arena_alloc(arena, 256);

    if (numbers && str) {
        for (int i = 0; i < 10; i++) {
            numbers[i] = i * 15;
        }
        snprintf(str, 256, "Hello from nx-core zero-fragmentation arena allocator!");
        
        printf("[+] Data allocated & written successfully:\n");
        printf("    numbers[5] = %d\n", numbers[5]);
        printf("    str = %s\n", str);
    }

    printf("[*] Resetting arena offset instantly (O(1) reset)...\n");
    nx_arena_reset(arena);

    // استفاده مجدد بلافاصله بعد از ریست
    char *str2 = (char*)nx_arena_alloc(arena, 128);
    if (str2) {
        snprintf(str2, 128, "Reusing arena memory instantly after zero-cost reset.");
        printf("[+] Re-allocated after reset: %s\n", str2);
    }

    nx_arena_destroy(arena);
    printf("[+] Arena destroyed and resources unmapped cleanly.\n");
    return 0;
}
