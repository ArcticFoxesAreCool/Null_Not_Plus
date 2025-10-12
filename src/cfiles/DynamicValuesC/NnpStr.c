#include "../../headers/datatypes.h"



uint8_t NnpStrEq(const NnpStr* str1, const NnpStr* str2){
    if (str1->union_mode == NNPSTR_UNIONMODE_BUFFER && str2->union_mode == NNPSTR_UNIONMODE_BUFFER){
        return (strncmp(str1->string.buffer, str2->string.buffer, STRING_U_BUFFER_SIZE) == 0);
    }
    
    else if (str1->union_mode == NNPSTR_UNIONMODE_POINTER && str2->union_mode == NNPSTR_UNIONMODE_POINTER){
        return (strcmp(str1->string.dyn_str, str2->string.dyn_str) == 0);
    }
    
    return false;
}


NnpStr makeNnpStr(const char* str){
    assert(str);
    NnpStr st = {.union_mode = NNPSTR_UNIONMODE_BUFFER, .string.buffer = ""};
    setNnpStr(str, &st);
    return st;
}


void setNnpStr(const char* str, NnpStr* p_nnpStr){
    assert(str && p_nnpStr);
    if (p_nnpStr->union_mode == NNPSTR_UNIONMODE_POINTER){
        free(p_nnpStr->string.dyn_str);
    }

    if (strlen(str) <= STRING_U_BUFFER_NUM_CHARS){
        p_nnpStr->union_mode = NNPSTR_UNIONMODE_BUFFER;
        strncpy(p_nnpStr->string.buffer, str, STRING_U_BUFFER_SIZE);
    } else {
        p_nnpStr->union_mode = NNPSTR_UNIONMODE_POINTER;
        p_nnpStr->string.dyn_str = malloc(strlen(str) + 1);
        assert(p_nnpStr->string.dyn_str);
        strncpy(p_nnpStr->string.dyn_str, str, strlen(str) + 1);
    }

}

void freeNnpStr(NnpStr* p_nnpStr){
    if (p_nnpStr->union_mode == NNPSTR_UNIONMODE_BUFFER){
        p_nnpStr->string.buffer[0] = '\0';
        return;
    }

    free(p_nnpStr->string.dyn_str);

    p_nnpStr->union_mode = NNPSTR_UNIONMODE_BUFFER;
    p_nnpStr->string.buffer[0] = '\0';
}

void catNnpStrs(NnpStr* src1, const NnpStr* src2){
    assert(src1 && src2);

    uint src1_length;
    if (  src1->union_mode == NNPSTR_UNIONMODE_BUFFER  ){
        src1_length = strnlen(  src1->string.buffer, STRING_U_BUFFER_SIZE  );
    } else {
        src1_length = strlen(  src1->string.dyn_str  );
    }

    uint src2_length;
    if (src2->union_mode == NNPSTR_UNIONMODE_BUFFER){
        src2_length = strnlen(src2->string.buffer, STRING_U_BUFFER_SIZE);
    } else {
        src2_length = strlen(src2->string.dyn_str);
    }


    char buffer[src1_length + src2_length + 1];
    
    if (  src1->union_mode == NNPSTR_UNIONMODE_BUFFER  ){
        strncpy(buffer, src1->string.buffer, src1_length + 1);
    } else {
        strncpy(buffer, src1->string.dyn_str, src1_length + 1);
    }
    
    if (  src2->union_mode == NNPSTR_UNIONMODE_BUFFER  ){
        strncat(buffer, src2->string.buffer, src2_length);
    } else {
        strncat(buffer, src2->string.dyn_str, src2_length);
    }

    setNnpStr(buffer, src1);
}

