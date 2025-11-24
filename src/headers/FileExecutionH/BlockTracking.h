#ifndef BLOCKTRACKING_H
#define BLOCKTRACKING_H

#define BLOCK_TRACKER_INITIAL_CAPACITY 8

enum BlockState {
    BLOCK_DEFAULT,
    BLOCK_IF_HAS_TRIGGERED,
    BLOCK_IF_NOT_TRIGGERED,
    BLOCK_NOW_WILL_EXECUTE,
    BLOCK_NOW_NOT_EXECUTE,

    BLOCK_LOOP_LEAVE,
    BLOCK_LOOP_REEXECUTE
};

struct BlockData {
    long loop_tell;
    enum BlockState state;
};

struct BlockTracker {
    struct BlockData* data;
    long loop_tells;
    int capacity;
    int length;
};

void initBlockTracker(struct BlockTracker* p_block_tracker, int initial_capacity);

void freeBlockTracker(struct BlockTracker* p_block_tracker);

void appendBlockTracker(struct BlockTracker* p_block_tracker, struct BlockData new_data);

void popBlocktracker(struct BlockTracker* p_block_tracker);

void returnToLoop(const struct BlockTracker* p_block_tracker);

#endif