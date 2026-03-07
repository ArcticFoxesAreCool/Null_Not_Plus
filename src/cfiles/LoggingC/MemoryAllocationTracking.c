#include "../../headers/datatypes.h"

static struct AllocTracking alloc_tracker = {
    .num_malloc = 0,
    .num_calloc = 0,
    .num_realloc = 0,
    .num_free = 0,
    .mallocAndCallocBytes = 0
};


void* myMalloc(size_t num_bytes){
    alloc_tracker.num_malloc++;
    alloc_tracker.mallocAndCallocBytes += num_bytes;

#if 0
    void* r = malloc(num_bytes);
    printf("\tMalloc: %p\n", r);fflush(stdout);
    return r;
#else
    return malloc(num_bytes);
#endif
}

void* myCalloc(size_t count, size_t size){
    alloc_tracker.num_calloc++;
    alloc_tracker.mallocAndCallocBytes += count * size;

    return calloc(count, size);
}

void* myRealloc(void* ptr, size_t new_size){
    alloc_tracker.num_realloc++;
    return realloc(ptr, new_size);
}


void myFree(void* ptr){
    alloc_tracker.num_free++;
#if 0
    printf("\tFree:   %p\n", ptr);fflush(stdout);
#endif
    free(ptr);
}


void logAllocations(enum LoggingFileType file_type){
    logMessage(file_type, 
        "\nAllocation Tracking\n"
        "\tMallocs: %d\n"
        "\tCallocs: %d\n"
        "\tReallocs: %d\n"
        "\tFrees: %d\n"
        "\tMalloc and Calloc Bytes Allocated: %d\n"
        "Net allocations: %d\n\n",
        alloc_tracker.num_malloc, alloc_tracker.num_calloc, alloc_tracker.num_realloc, alloc_tracker.num_free, alloc_tracker.mallocAndCallocBytes,
        (alloc_tracker.num_malloc + alloc_tracker.num_calloc - alloc_tracker.num_free)
    );
}