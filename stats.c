#include <stdio.h>

static int total = 0;
static int page_faults = 0;
static int tlb_hits = 0;
static int dirty_writebacks = 0;  // NEW: track dirty evictions

void init_stats() {
    total = page_faults = tlb_hits = dirty_writebacks = 0;
}

void update_total_addresses() { total++; }
void update_page_faults() { page_faults++; }
void update_tlb_hits() { tlb_hits++; }
void update_dirty_writebacks() { dirty_writebacks++; }  // NEW

void print_stats() {
    printf("\n--- Statistics ---\n");
    printf("Total addresses: %d\n", total);
    printf("Page faults: %d (%.2f%%)\n", page_faults, 100.0 * page_faults / total);
    printf("TLB hits: %d (%.2f%%)\n", tlb_hits, 100.0 * tlb_hits / total);
    printf("Dirty pages written back: %d\n", dirty_writebacks);  // NEW
}
