
#include "../../headers/datatypes.h"


void initBlockTracker(struct BlockTracker* p_block_tracker, int initial_capacity){
    assert(p_block_tracker && p_block_tracker->capacity == 0 && p_block_tracker->length == 0 && p_block_tracker->data == NULL);
    assert(initial_capacity >= 1);

    p_block_tracker->capacity = initial_capacity;
    p_block_tracker->data = myMalloc(initial_capacity * sizeof(struct BlockData));
    assert(p_block_tracker->data);
}

void freeBlockTracker(struct BlockTracker* p_block_tracker){
    assert(p_block_tracker && p_block_tracker->capacity >= 1 && p_block_tracker->data);
    if (p_block_tracker->length != 0){
        logMessage(OUT, "\nMissed Block Resolution. Last State: %d, Final Length: %d\n", p_block_tracker->data[p_block_tracker->length-1].state, p_block_tracker->length);
    }
    myFree(p_block_tracker->data);
    p_block_tracker->data = NULL;
    p_block_tracker->capacity = 0;
    p_block_tracker->length = 0;
}

void appendBlockTracker(struct BlockTracker* p_block_tracker, struct BlockData new_data){
    assert(p_block_tracker && p_block_tracker->capacity >= 1 && p_block_tracker->data);
    if (p_block_tracker->length == p_block_tracker->capacity){
        p_block_tracker->capacity *= 2;
        p_block_tracker->data = myRealloc(p_block_tracker->data, p_block_tracker->capacity * sizeof(struct BlockData));
        assert(p_block_tracker->data);
    }
    p_block_tracker->data[p_block_tracker->length++] = new_data;
}

void popBlocktracker(struct BlockTracker* p_block_tracker){
    assert(p_block_tracker && p_block_tracker->capacity >= 1 && p_block_tracker->data && p_block_tracker->length >= 1);
    p_block_tracker->length--;
    // printf("Poped len: %d, state: %d\n", p_block_tracker->length, p_block_tracker->data[p_block_tracker->length].state);
}

void returnToLoop(const struct BlockTracker* p_block_tracker){
    assert(p_block_tracker && p_block_tracker->capacity >= 1 && p_block_tracker->length >= 1 && p_block_tracker->data && p_block_tracker->data[p_block_tracker->length - 1].state == BLOCK_LOOP_REEXECUTE);
    extern FILE* nnp_code;
    fseek(nnp_code, p_block_tracker->data[p_block_tracker->length - 1].loop_tell, SEEK_SET);
}