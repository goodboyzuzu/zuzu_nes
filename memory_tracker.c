#include <stdlib.h> // For original malloc/free
#include <stdio.h>  // For printing messages
#include <string.h> // For memset (optional, for safety)

// Structure to hold allocated memory information
typedef struct MemoryBlock {
    void *ptr;
    size_t size;
    struct MemoryBlock *next;
} MemoryBlock;

// Head of the linked list of allocated blocks
static MemoryBlock *allocated_blocks = NULL;

// Wrapper for malloc
void* my_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr) {
        MemoryBlock *new_block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
        if (new_block) {
            new_block->ptr = ptr;
            new_block->size = size;
            new_block->next = allocated_blocks;
            allocated_blocks = new_block;
            // Optionally, fill with a known pattern to detect use-after-free
            // memset(ptr, 0xAA, size); 
            debug_printf("Allocated %zu bytes at %p\n", size, ptr);
        } else {
            // Handle allocation failure for tracking structure itself
            free(ptr); // Free the original allocation if tracking fails
            fprintf(stderr, "Error: Failed to allocate memory for tracking block.\n");
            return NULL;
        }
    } else {
        fprintf(stderr, "Error: malloc failed to allocate %zu bytes.\n", size);
    }
    return ptr;
}

// Wrapper for free
void my_free(void *ptr) {
    if (!ptr) {
        return; // Freeing NULL is safe
    }

    MemoryBlock *current = allocated_blocks;
    MemoryBlock *prev = NULL;

    while (current) {
        if (current->ptr == ptr) {
            if (prev) {
                prev->next = current->next;
            } else {
                allocated_blocks = current->next;
            }
            // Optionally, clear freed memory to detect use-after-free
            // memset(current->ptr, 0xDD, current->size); 
            free(current->ptr); // Free the actual memory
            free(current); // Free the tracking block
            debug_printf("Freed memory at %p\n", ptr);
            return;
        }
        prev = current;
        current = current->next;
    }
    fprintf(stderr, "Warning: Attempted to free untracked or already freed memory at %p\n", ptr);
}

// Function to check for unfreed memory at program exit
void check_memory_leaks() {
    MemoryBlock *current = allocated_blocks;
    if (current) {
        fprintf(stderr, "Memory Leaks Detected:\n");
        while (current) {
            fprintf(stderr, "  - Unfreed block at %p, size %zu bytes\n", current->ptr, current->size);
            MemoryBlock *temp = current;
            current = current->next;
            free(temp->ptr); // Free remaining blocks for clean exit
            free(temp);
        }
    } else {
        debug_printf("No memory leaks detected.\n");
    }
}