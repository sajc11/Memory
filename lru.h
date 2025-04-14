#ifndef LRU_H
#define LRU_H

void init_lru(int frame_count);
int get_lru_frame();
void mark_lru_used(int frame);

#endif
