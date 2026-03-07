#ifndef USERFUNCTIONEXECUTION_H
#define USERFUNCTIONEXECUTION_H

#include "../DynamicValuesH/Storage.h"

#define INITIAL_CALL_STACK_CAPACITY 4



typedef struct {
    int length;
    int capacity;
    
    int* previous_line;
    Storage* local_vars;
    long* where_will_return;
} CallStack;

void initCallStack(int capacity);

void freeCallStack();

void appendCallStack(long tell, int previous_line);

void popAndFSeekCallStack();

void resolveUserFunc(ObjArray* p_obj_arr, int num_args);

#endif