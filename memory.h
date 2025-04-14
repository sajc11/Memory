#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>  // REQUIRED for 'FILE*'

void init_memory(FILE *backing_store_fp);
void handle_address(int full_address);
void close_memory();

#endif
