#include "../../headers/datatypes.h"



void initLoopList(struct LoopList* p_loop_list, int initial_size){
    assert(p_loop_list && p_loop_list->data == NULL && p_loop_list->capacity == 0 && p_loop_list->length == 0);
    assert(initial_size > 0);

    p_loop_list->capacity = initial_size;
    p_loop_list->data = myMalloc(sizeof(struct LoopListData) * p_loop_list->capacity);
    assert(p_loop_list->data);
}

void freeLoopList(struct LoopList* p_loop_list){
    assert(p_loop_list && p_loop_list->data && p_loop_list->capacity >= 1);
    myFree(p_loop_list->data);
    p_loop_list->capacity = 0;
    p_loop_list->length = 0;
    p_loop_list->data = NULL;
}

void appendLoopList(struct LoopList* p_loop_list, struct LoopListData loop_data){
    assert(p_loop_list && p_loop_list->data && p_loop_list->capacity >= 1);
    if (p_loop_list->capacity == p_loop_list->length){
        p_loop_list->capacity *= 2;
        p_loop_list->data = myRealloc(p_loop_list->data, sizeof(struct LoopListData) * p_loop_list->capacity);
        assert(p_loop_list->data);
    }
    // p_loop_list->data[p_loop_list->length - 1].line_number = loop_data.line_number;
    // p_loop_list->data[p_loop_list->length - 1].loop_tell = loop_data.loop_tell;
    // p_loop_list->data[p_loop_list->length - 1].no_scope_level = loop_data.no_scope_level;
    p_loop_list->length++;
}

void popLoopList(struct LoopList* p_loop_list){
    assert(p_loop_list && p_loop_list->data && p_loop_list->capacity >= 1 && p_loop_list->length >= 1);
    p_loop_list->length--;
}



void gotoCurrentLoop(const struct LoopList* p_loop_list){
    assert(p_loop_list && p_loop_list->capacity >= 1 && p_loop_list->length >= 1 && p_loop_list->data);
    extern FILE* nnp_code;

    fseek(nnp_code, p_loop_list->data[p_loop_list->length - 1].loop_tell, SEEK_SET);
}