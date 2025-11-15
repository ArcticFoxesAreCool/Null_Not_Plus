#ifndef MEMORYALLOCATIONTRACKING_H
#define MEMORYALLOCATIONTRACKING_H

#include <stdlib.h>
#include "../LoggingH/LogMessage.h"

void* myMalloc(size_t num_bytes);

void* myCalloc(size_t count, size_t size);

void myFree(void* ptr);

void* myRealloc(void* ptr, size_t new_size);


void logAllocations(enum LoggingFileType file_type);

struct AllocTracking {
    int num_malloc;
    int num_calloc;
    int num_realloc;
    int num_free;
    uint mallocAndCallocBytes;
};


#endif