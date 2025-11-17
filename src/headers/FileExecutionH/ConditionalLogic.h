#ifndef CONDITIONALLOGIC_H
#define CONDITIONALLOGIC_H

#include <stdio.h>
#include <inttypes.h>

// #define NO_TRACKER_IF_HAS_NOT_TRIGGERED 1
// #define NO_TRACKER_IF_HAS_TRIGGERED     2
// #define NO_TRACKER_LOOP                 3

struct ConditionalData {
    bool if_has_been_triggered;
    int no_scope_level;
};

struct ConditionalTracking {
    struct ConditionalData* data;

    int length;
    int capacity;
};


void initConditionalTracker(struct ConditionalTracking* p_tracker,int initial_capacity);

void freeConditionalTracker(struct ConditionalTracking* p_tracker);

void appendToConditionalTracker(struct ConditionalTracking* p_tracker, struct ConditionalData data);

void popConditionalTracker(struct ConditionalTracking* p_tracker);

#endif