#ifndef LOOPTRACKING_H
#define LOOPTRACKING_H

#include <stdio.h>

struct LoopListData {
    long loop_tell;
    int no_scope_level;
    int line_number;
};

struct LoopList {
    struct LoopListData* data;
    int length;
    int capacity;
};

void initLoopList(struct LoopList* p_loop_list, int initial_size);

void freeLoopList(struct LoopList* p_loop_list);

void appendLoopList(struct LoopList* p_loop_list, struct LoopListData loop_data);

void popLoopList(struct LoopList* p_loop_list);

void gotoCurrentLoop(const struct LoopList* p_loop_list);


#endif