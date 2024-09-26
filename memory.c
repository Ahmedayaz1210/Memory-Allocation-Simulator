#include "memory_allocation.h"
#include <limits.h>

#define NULLBLOCK (struct MEMORY_BLOCK) {0, 0, 0, 0}

struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id) {
    // This method allocates memory according to the Best Fit scheme.
    int best_fit_index = -1;
    int smallest_suitable_size = INT_MAX;

    // It finds the candidate memory blocks that can be allocated and chooses the one whose size is closest to the requested size.
    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size) {
            if (memory_map[i].segment_size < smallest_suitable_size) {
                smallest_suitable_size = memory_map[i].segment_size;
                best_fit_index = i;
            }
        }
    }

    // If there is no free block of memory (in the memory map) that is at least as large as the requested size, the method returns the NULLBLOCK.
    if (best_fit_index == -1) {
        return NULLBLOCK;
    }

    struct MEMORY_BLOCK allocated_block = memory_map[best_fit_index];
    allocated_block.process_id = process_id;
    allocated_block.segment_size = request_size;
    allocated_block.end_address = allocated_block.start_address + request_size - 1;

    // If the free block found is larger than the requested size, the block is split into two pieces - the first piece allocated and the second piece becoming a free block in the memory map.
    if (memory_map[best_fit_index].segment_size > request_size) {
        struct MEMORY_BLOCK remaining_block = {
            allocated_block.end_address + 1,
            memory_map[best_fit_index].end_address,
            memory_map[best_fit_index].segment_size - request_size,
            0
        };

        memory_map[best_fit_index] = allocated_block;
        for (int i = *map_cnt; i > best_fit_index + 1; i--) {
            memory_map[i] = memory_map[i-1];
        }
        memory_map[best_fit_index + 1] = remaining_block;
        (*map_cnt)++;
    } else {
        // If the free block found is exactly of the same size as the requested size, the method updates the process id to allocate it and returns this memory block.
        memory_map[best_fit_index] = allocated_block;
    }

    return allocated_block;
}

struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id) {
    // This method allocates memory according to the First Fit scheme.

    // It finds the first (lowest starting address) free memory block whose size is at least as large as the requested size.
    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size) {
            struct MEMORY_BLOCK allocated_block = memory_map[i];
            allocated_block.process_id = process_id;
            allocated_block.segment_size = request_size;
            allocated_block.end_address = allocated_block.start_address + request_size - 1;

            // If the free block found is larger than the requested size, the block is split into two pieces - the first piece allocated and the second piece becoming a free block in the memory map.
            if (memory_map[i].segment_size > request_size) {
                struct MEMORY_BLOCK remaining_block = {
                    allocated_block.end_address + 1,
                    memory_map[i].end_address,
                    memory_map[i].segment_size - request_size,
                    0
                };

                memory_map[i] = allocated_block;
                for (int j = *map_cnt; j > i + 1; j--) {
                    memory_map[j] = memory_map[j-1];
                }
                memory_map[i + 1] = remaining_block;
                (*map_cnt)++;
            } else {
                // If the free block found is exactly of the same size as the requested size, the method updates the process id to allocate it and returns this memory block.
                memory_map[i] = allocated_block;
            }

            return allocated_block;
        }
    }

    // If there is no free block of memory (in the memory map) that is at least as large as the requested size, the method returns the NULLBLOCK.
    return NULLBLOCK;
}

struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id) {
    // This method allocates memory according to the Worst Fit scheme.

    int worst_fit_index = -1;
    int largest_suitable_size = 0;

    // It finds the candidate memory blocks that can be allocated and chooses the largest among these blocks.
    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size) {
            if (memory_map[i].segment_size > largest_suitable_size) {
                largest_suitable_size = memory_map[i].segment_size;
                worst_fit_index = i;
            }
        }
    }

    // If there is no free block of memory (in the memory map) that is at least as large as the requested size, the method returns the NULLBLOCK.
    if (worst_fit_index == -1) {
        return NULLBLOCK;
    }

    struct MEMORY_BLOCK allocated_block = memory_map[worst_fit_index];
    allocated_block.process_id = process_id;
    allocated_block.segment_size = request_size;
    allocated_block.end_address = allocated_block.start_address + request_size - 1;

    // If the free block found is larger than the requested size, the block is split into two pieces - the first piece allocated and the second piece becoming a free block in the memory map.
    if (memory_map[worst_fit_index].segment_size > request_size) {
        struct MEMORY_BLOCK remaining_block = {
            allocated_block.end_address + 1,
            memory_map[worst_fit_index].end_address,
            memory_map[worst_fit_index].segment_size - request_size,
            0
        };

        memory_map[worst_fit_index] = allocated_block;
        for (int i = *map_cnt; i > worst_fit_index + 1; i--) {
            memory_map[i] = memory_map[i-1];
        }
        memory_map[worst_fit_index + 1] = remaining_block;
        (*map_cnt)++;
    } else {
        // If the free block found is exactly of the same size as the requested size, the method updates the process id to allocate it and returns this memory block.
        memory_map[worst_fit_index] = allocated_block;
    }

    return allocated_block;
}

struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id, int last_address) {
    // This method allocates memory according to the Next Fit scheme.

    // It finds the first (lowest starting address) free memory block, greater than or equal to the previously allocated block address, whose size is at least as the requested size.
    int start_index = 0;
    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].start_address > last_address) {
            start_index = i;
            break;
        }
    }

    for (int i = 0; i < *map_cnt; i++) {
        int current_index = (start_index + i) % *map_cnt;
        if (memory_map[current_index].process_id == 0 && memory_map[current_index].segment_size >= request_size) {
            struct MEMORY_BLOCK allocated_block = memory_map[current_index];
            allocated_block.process_id = process_id;
            allocated_block.segment_size = request_size;
            allocated_block.end_address = allocated_block.start_address + request_size - 1;

            // If the free block found is larger than the requested size, the block is split into two pieces - the first piece allocated and the second piece becoming a free block in the memory map.
            if (memory_map[current_index].segment_size > request_size) {
                struct MEMORY_BLOCK remaining_block = {
                    allocated_block.end_address + 1,
                    memory_map[current_index].end_address,
                    memory_map[current_index].segment_size - request_size,
                    0
                };

                memory_map[current_index] = allocated_block;
                for (int j = *map_cnt; j > current_index + 1; j--) {
                    memory_map[j] = memory_map[j-1];
                }
                memory_map[current_index + 1] = remaining_block;
                (*map_cnt)++;
            } else {
                // If the free block found is exactly of the same size as the requested size, the method updates the process id to allocate it and returns this memory block.
                memory_map[current_index] = allocated_block;
            }

            return allocated_block;
        }
    }

    // If there is no free block of memory (in the memory map) that is at least as large as the requested size, the method returns the NULLBLOCK.
    return NULLBLOCK;
}

void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt) {
    // This method releases a memory block. Accordingly, it modifies the memory map passed in.
    int freed_index = -1;

    // Find the freed block in the memory map
    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].start_address == freed_block.start_address &&
            memory_map[i].end_address == freed_block.end_address) {
            freed_index = i;
            // Specifically, it marks the released block of memory as free
            memory_map[i].process_id = 0;
            break;
        }
    }

    if (freed_index == -1) return;  // Block not found

    // Then it merges that block with adjacent free blocks if any.
    // Merge with previous block if it's free
    if (freed_index > 0 && memory_map[freed_index - 1].process_id == 0) {
        memory_map[freed_index - 1].end_address = memory_map[freed_index].end_address;
        memory_map[freed_index - 1].segment_size += memory_map[freed_index].segment_size;
        
        // Shift the array to remove the merged block
        for (int i = freed_index; i < *map_cnt - 1; i++) {
            memory_map[i] = memory_map[i + 1];
        }
        (*map_cnt)--;
        freed_index--;
    }

    // Merge with next block if it's free
    if (freed_index < *map_cnt - 1 && memory_map[freed_index + 1].process_id == 0) {
        memory_map[freed_index].end_address = memory_map[freed_index + 1].end_address;
        memory_map[freed_index].segment_size += memory_map[freed_index + 1].segment_size;
        
        // Shift the array to remove the merged block
        for (int i = freed_index + 1; i < *map_cnt - 1; i++) {
            memory_map[i] = memory_map[i + 1];
        }
        (*map_cnt)--;
    }
}