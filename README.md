---

# Virtual Memory Simulation

## Overview

This project simulates a virtual memory system supporting:
- Logical-to-physical address translation
- A Translation Lookaside Buffer (TLB) with FIFO policy
- Page fault handling and demand paging from a binary `BACKING_STORE.bin`
- Least Recently Used (LRU) page replacement for frames
- Dirty page detection and write-back
- Input validation with support for write-flagged addresses
- Detailed runtime statistics

It models how an operating system handles virtual memory access under constraints of limited physical memory.

---

## Features Summary

- Logical address parsing and write flag support (bit 16)
- TLB lookup and FIFO replacement (16 entries)
- Page table and memory residency tracking
- LRU-based frame eviction policy
- Dirty page handling with write-back to backing store
- Input validation and error handling
- Modular architecture
- Test suite for correctness and edge cases

---

## Project Structure

```
virtual_memory/
├── main.c                  # Entry point: reads addresses, handles loop
├── memory.c / memory.h     # Address translation and memory logic
├── lru.c / lru.h           # LRU frame tracking
├── stats.c / stats.h       # Runtime statistics
├── utils.c / utils.h       # Address parsing and validation
├── Makefile                # Build automation
├── BACKING_STORE.bin       # 65,536-byte binary page store
├── addresses.txt           # Sample logical address input file
├── run_all_tests.sh        # Test runner for all test_*.sh files
├── test_*.sh               # Individual test scripts
└── README.md               # This documentation
```

---

## Build Instructions

To compile the project:

```bash
make
```

This will produce the `vm` executable.

To clean up build artifacts:

```bash
make clean
```

---

## Running the Simulation

```bash
./vm addresses.txt
```

Each address in `addresses.txt` is a 32-bit integer:
- Lower 16 bits: logical address (page number and offset)
- Bit 16: write flag (1 = write; 0 = read)

---

## Memory Configuration

| Component                | Value                       |
|--------------------------|-----------------------------|
| Page size                | 256 bytes                   |
| Number of pages          | 256                         |
| Physical memory size     | 32 KB (128 frames)          |
| TLB size                 | 16 entries (FIFO policy)    |
| Frame replacement policy | LRU                         |
| Logical address space    | 2¹⁶ = 65,536 bytes          |

---

## Simulation Workflow

1. Parse 32-bit input address:
   - Extract logical address (bits 0–15)
   - Extract write flag (bit 16)
2. Check TLB:
   - If page found: TLB hit
   - Otherwise: TLB miss
     - Check page table
     - If page not loaded: page fault
       - Load page from backing store
       - If memory is full, evict least recently used page
         - If evicted page is dirty, write back to disk
3. Update TLB using FIFO policy
4. Compute physical address: `frame_number * PAGE_SIZE + offset`
5. Simulate read or write
6. Log translation and value

Example output:

```
Logical: 0xC30A → Physical: 0x4A0A → Value: 45 [WRITE]
```

---

## Output Statistics

At the end of execution, the following statistics are printed:

```
--- Statistics ---
Total addresses: 1000
Page faults: 537 (53.70%)
TLB hits: 67 (6.70%)
```

---

## Testing

Run all tests using:

```bash
./run_all_tests.sh
```

### Included Tests

| Script                  | Description                                   |
|--------------------------|-----------------------------------------------|
| `test_basic.sh`          | Verifies translation, TLB hits, and writes    |
| `test_dirty_eviction.sh` | Tests dirty page handling and write-back      |
| `test_invalid_input.sh`  | Rejects malformed or out-of-range addresses   |
| `test_lru_eviction.sh`   | Confirms eviction occurs with >128 pages      |

Test output includes `[PASS]` or `[FAIL]` indicators.

---

## Enhancements Implemented

- Input validation with whitespace trimming and numeric range checking
- Dirty page detection and write-back implementation
- LRU eviction policy for memory frames
- Modular parsing and logic for clarity
- Clean formatted statistics with percentage breakdowns
- Test framework with auto-discovery and validation checks

---

## Potential Future Work

- Replace TLB FIFO with LRU or second-chance algorithm
- Add memory-mapped I/O support (`mmap`)
- Integrate a visualization of memory system state
- Implement page prefetching or adaptive replacement strategies

---

## Notes

- The `BACKING_STORE.bin` must contain exactly 256 pages, each 256 bytes long
- Write operations only affect simulated memory; disk writes occur only during dirty page eviction
- Simulator ensures safety with boundary checks and overflow protections

---