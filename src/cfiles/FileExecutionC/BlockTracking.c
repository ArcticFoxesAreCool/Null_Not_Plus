
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
    if (p_block_tracker->data[p_block_tracker->length - 1].loop_label)
        freeObj(p_block_tracker->data[p_block_tracker->length - 1].loop_label);
    p_block_tracker->length--;
    // printf("Poped len: %d, state: %d\n", p_block_tracker->length, p_block_tracker->data[p_block_tracker->length].state);
}

void returnToLoop(const struct BlockTracker* p_block_tracker){
    assert(p_block_tracker && p_block_tracker->capacity >= 1 && p_block_tracker->length >= 1 && p_block_tracker->data && p_block_tracker->data[p_block_tracker->length - 1].state == BLOCK_LOOP_REEXECUTE);
    extern FILE* nnp_code;

    extern int line_number;
    
    line_number = p_block_tracker->data[p_block_tracker->length - 1].line_count;
    fseek(nnp_code, p_block_tracker->data[p_block_tracker->length - 1].loop_tell, SEEK_SET);
}



enum LoopBreakContinue loopLineIsBreakContinue(){
    extern Reader nian;
    extern TokenTyper tok_types;

    assert(nian.charv && nian.sz > 0 && nian.token_indexes && nian.tok_ind_capacity > 0 && nian.tok_ind_len > 0);
    assert(tok_types.size > 0 && tok_types.types);

    for(int i = 0; i < nian.tok_ind_len - 1; i++){
        if (strncmp(nian.charv + nian.token_indexes[i], "loop", 5) == 0){
            i++;
            if (strncmp(nian.charv + nian.token_indexes[i], "->", 3) == 0){
                return LOOP_BREAK;
            } else if (strncmp(nian.charv + nian.token_indexes[i], "<-", 3) == 0){
                return LOOP_CONTINUE;
            }
            return LOOP_START;
        }
    }
    assert("No loop found when loopLineIsBreak is called" && false);
    exit(1);
    return LOOP_START;
}



object_p getLoopsLabel(){
    extern Reader nian;
    extern TokenTyper tok_types;
    assert(nian.charv && nian.sz > 0 && nian.token_indexes && nian.tok_ind_capacity > 0 && nian.tok_ind_len > 0);
    assert(tok_types.size > 0 && tok_types.types);

    object_p ret;
    for(int i = 1; i < nian.tok_ind_len; i++){
        if (strncmp(nian.charv + nian.token_indexes[i], "loop", 5) == 0){
            ObjArray temp_obj_arr = {.capacity = DEFAULT_OBJ_ARRAY_CAPACITY, .length = 0, .objs = myMalloc(sizeof(object_p) * DEFAULT_OBJ_ARRAY_CAPACITY)};
            assert(temp_obj_arr.objs);
            subCondenseObjsOperators(&temp_obj_arr, NULL, 0, i - 1);


            for(uint j = 0; j < temp_obj_arr.length - 1; j++){
                freeObj(temp_obj_arr.objs[j]);
            }
            ret = temp_obj_arr.objs[temp_obj_arr.length - 1];

            myFree(temp_obj_arr.objs);
            return ret;
        }
    }

    return NULL;
}

