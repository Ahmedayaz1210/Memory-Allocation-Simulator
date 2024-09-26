# Memory Allocation Simulator

This project implements various memory allocation algorithms commonly used in operating systems. It simulates how an OS might manage memory blocks, allocate them to processes, and release them when no longer needed.

## Algorithms Implemented

1. **Best Fit Allocation**: Finds the smallest free block that can accommodate the request.
2. **First Fit Allocation**: Allocates the first free block that can accommodate the request.
3. **Worst Fit Allocation**: Finds the largest free block that can accommodate the request.
4. **Next Fit Allocation**: Similar to First Fit, but starts searching from the last allocated block.
5. **Memory Release**: Frees a memory block and merges it with adjacent free blocks if possible.

## Key Features

- Simulates a memory map as an array of memory blocks.
- Each memory block tracks its start address, end address, size, and the process ID using it.
- Handles scenarios such as exact fits, splitting larger blocks, and cases where no suitable block is found.
- Implements memory coalescing when releasing memory.

## How It Works

Each allocation algorithm searches the memory map for a suitable free block based on its specific criteria. When a block is found:

- If it's an exact fit, it's allocated directly to the process.
- If it's larger than needed, it's split into an allocated portion and a remaining free portion.
- If no suitable block is found, a null block is returned.

The memory release function frees a given block and merges it with adjacent free blocks to reduce fragmentation.

## Usage

Include the `memory_allocation.h` header in your project and call the functions as needed:

```c
struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id);
struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id);
struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id);
struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id, int last_address);
void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt);
```
