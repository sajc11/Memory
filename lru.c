#include "lru.h"
#include <limits.h>

#define MAX_FRAMES 128

static int last_used[MAX_FRAMES];
static int tick = 0;
static int frames = 0;

void init_lru(int frame_count) {
    frames = frame_count;
    for (int i = 0; i < frames; i++) last_used[i] = -1;
    tick = 0;
}

void mark_lru_used(int frame) {
    last_used[frame] = tick++;
}

int get_lru_frame() {
    int min = INT_MAX, victim = 0;
    for (int i = 0; i < frames; i++) {
        if (last_used[i] < min) {
            min = last_used[i];
            victim = i;
        }
    }
    return victim;
}
