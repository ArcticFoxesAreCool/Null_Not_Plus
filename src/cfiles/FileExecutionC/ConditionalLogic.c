#include "../../headers/datatypes.h"



void initConditionalTracker(struct ConditionalTracking* p_tracker,int initial_capacity){
    assert(p_tracker && p_tracker->data == NULL && p_tracker->length == 0 && p_tracker->capacity == 0);
    p_tracker->capacity = initial_capacity;
    p_tracker->data = myMalloc(initial_capacity * sizeof(struct ConditionalData));
    assert(p_tracker->data);
}

void freeConditionalTracker(struct ConditionalTracking* p_tracker){
    assert(p_tracker && p_tracker->data && p_tracker->capacity >= 1);
    myFree(p_tracker->data);
    p_tracker->capacity = 0;
    p_tracker->length = 0;
    p_tracker->data = NULL;
}

void appendToConditionalTracker(struct ConditionalTracking* p_tracker, struct ConditionalData data){
    assert(p_tracker && p_tracker->data && p_tracker->capacity >= 1);
    if (p_tracker->length == p_tracker->capacity){
        p_tracker->capacity *= 2;
        p_tracker->data = myRealloc(p_tracker->data, p_tracker->capacity * sizeof(struct ConditionalData));
        assert(p_tracker->data);
    }
    p_tracker->data[p_tracker->length] = data;
    p_tracker->length++;
}


void popConditionalTracker(struct ConditionalTracking* p_tracker){
    assert(p_tracker && p_tracker->data && p_tracker->capacity >= 1 && p_tracker->length >= 1);
    p_tracker->length--;
}