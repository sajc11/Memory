#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "stats.h"
#include "utils.h"  // ← your new parsing utility

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s addresses.txt\n", argv[0]);
        return 1;
    }

    FILE *backing_store = fopen("BACKING_STORE.bin", "rb+");
    if (!backing_store) {
        perror("BACKING_STORE.bin");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        perror(argv[1]);
        return 1;
    }

    init_memory(backing_store);

    char line[128];
    int addr;
    while (fgets(line, sizeof(line), input)) {
        if (parse_address_line(line, &addr)) {
            handle_address(addr);
        }
    }

    print_stats();
    close_memory();
    fclose(input);
    return 0;
}
