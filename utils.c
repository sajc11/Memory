#include "utils.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int parse_address_line(const char *line, int *addr_out) {
    if (!line || !addr_out) return 0;

    char *endptr;
    long parsed = strtol(line, &endptr, 10);

    // Skip trailing whitespace
    while (*endptr == ' ' || *endptr == '\t') endptr++;

    if (line == endptr || (*endptr != '\n' && *endptr != '\0')) {
        fprintf(stderr, "[WARN] Invalid input: non-numeric or trailing characters: %s", line);
        return 0;
    }

    if (parsed < 0 || parsed > UINT32_MAX) {
        fprintf(stderr, "[WARN] Address out of range: %s", line);
        return 0;
    }

    *addr_out = (int)parsed;
    return 1;
}
