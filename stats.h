#ifndef STATS_H
#define STATS_H

void init_stats();
void update_page_faults();
void update_tlb_hits();
void update_total_addresses();
void print_stats();
void update_dirty_writebacks();  

#endif
