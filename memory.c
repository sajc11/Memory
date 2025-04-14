#include "memory.h"
#include "lru.h"
#include "stats.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define TLB_SIZE 16
#define PAGE_SIZE 256
#define FRAME_COUNT 128
#define PAGE_TABLE_SIZE 256
#define MEMORY_SIZE (FRAME_COUNT * PAGE_SIZE)

typedef struct {
    int page;
    int frame;
} TLBEntry;

// Memory and tracking structures
static char memory[MEMORY_SIZE];
static int page_table[PAGE_TABLE_SIZE];   // -1 = not loaded
static int dirty[PAGE_TABLE_SIZE];        // 1 if page has been written to
static int frame_table[FRAME_COUNT];      // frame index → page number
static TLBEntry tlb[TLB_SIZE];
static int tlb_index = 0;

static FILE *backing_fp;

void init_memory(FILE *fp) {
    backing_fp = fp;

    memset(memory, 0, sizeof(memory));
    memset(page_table, -1, sizeof(page_table));
    memset(frame_table, -1, sizeof(frame_table));
    memset(dirty, 0, sizeof(dirty));

    for (int i = 0; i < TLB_SIZE; i++) {
        tlb[i].page = -1;
        tlb[i].frame = -1;
    }

    init_lru(FRAME_COUNT);
    init_stats();
}

static int get_page(int logical) { return (logical >> 8) & 0xFF; }
static int get_offset(int logical) { return logical & 0xFF; }
static int get_write(int logical) { return (logical >> 16) & 1; }

static int search_tlb(int page) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].page == page) {
            return tlb[i].frame;
        }
    }
    return -1;
}

static void update_tlb(int page, int frame) {
    tlb[tlb_index % TLB_SIZE] = (TLBEntry){page, frame};
    tlb_index++;
}

static int load_page(int page_num) {
    int frame = get_lru_frame();
    int evicted_page = frame_table[frame];

    // Evict if needed
    if (evicted_page != -1) {
        if (dirty[evicted_page]) {
            fseek(backing_fp, evicted_page * PAGE_SIZE, SEEK_SET);
            fwrite(&memory[frame * PAGE_SIZE], 1, PAGE_SIZE, backing_fp);
        }
        page_table[evicted_page] = -1;
        dirty[evicted_page] = 0;
    }

    // Load new page into memory
    fseek(backing_fp, page_num * PAGE_SIZE, SEEK_SET);
    fread(&memory[frame * PAGE_SIZE], 1, PAGE_SIZE, backing_fp);

    page_table[page_num] = frame;
    frame_table[frame] = page_num;
    mark_lru_used(frame);

    return frame;
}

void handle_address(int full_address) {
    static int debug_counter = 0;
    const int DEBUG_LIMIT = 1000000;

    int write = get_write(full_address);
    int logical = full_address & 0xFFFF;
    int page = get_page(logical);
    int offset = get_offset(logical);

    if (page < 0 || page >= PAGE_TABLE_SIZE || offset < 0 || offset >= PAGE_SIZE) {
        fprintf(stderr, "[ERROR] Invalid address: 0x%04X (page=%d, offset=%d)\n", logical, page, offset);
        return;
    }

    update_total_addresses();

    int frame = search_tlb(page);

    if (frame != -1) {
        update_tlb_hits();
    } else {
        frame = page_table[page];
        if (frame == -1) {
            update_page_faults();
            frame = load_page(page);
        }
        update_tlb(page, frame);
    }

    mark_lru_used(frame);

    int physical = frame * PAGE_SIZE + offset;
    unsigned char value = (unsigned char)memory[physical];

    if (write) {
        value += 1;
        memory[physical] = (char)value;
        dirty[page] = 1;
    }

    printf("Logical: 0x%04X → Physical: 0x%04X → Value: %d%s\n",
           logical, physical, value, write ? " [WRITE]" : "");

    if (++debug_counter > DEBUG_LIMIT) {
        fprintf(stderr, "[ERROR] Exceeded max address count (%d), exiting to prevent infinite loop.\n", DEBUG_LIMIT);
        exit(EXIT_FAILURE);
    }
}

void close_memory() {
    fflush(backing_fp);  // Ensure all dirty pages are flushed (optional)
    fclose(backing_fp);
}
