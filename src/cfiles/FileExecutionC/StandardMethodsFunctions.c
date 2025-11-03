#include "../../headers/datatypes.h"


void* speakFunction(StrObj* str){
    assert(str);
    if (str->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        printf("%s", str->value.string.buffer);
    } else {
        printf("%s", str->value.string.dyn_str);
    }
    return NULL;
}


StrObj* listenFunction(){
    uint length = 8;
    char* ear = malloc(length);
    fgets(ear, length, stdin);

    while (strnChOccur(ear, length, '\n') == NULL){
        ear = realloc(ear, length * 2);
        assert(ear);

        fgets(ear + length - 1, length + 1, stdin);
        length *= 2;
    }
    StrObj* ret = constructStrObj_char(ear);
    free(ear);
    return ret;
}

StrObj* asciiToChFunction(NumObj* num){
    assert(num);
    assert(num->value >= 0.0 && floor(num->value) <= 255.0);
    char num_as_u8 = (char)(num->value);

    char ch_buf[2] = {num_as_u8, '\0'};
    return constructStrObj_char(ch_buf);
}







int getFunctionFinalParameterIndex(int num_params, int larrow_index, int stop_index, const Storage* p_store){
    extern Reader nian; extern TokenTyper tok_types;
    assert(nian.charv && nian.sz > 0 && nian.tok_ind_capacity > 1 && nian.token_indexes && nian.tok_ind_len > 1);
    assert(tok_types.size > 1 && tok_types.types);
    assert(larrow_index <= stop_index);
    assert(nian.tok_ind_len > stop_index);
    // printf();
    if (num_params == 0){ return -1; }
    int count = 0;
    NnpStr temp_str = {0};
    FuncObj* temp_func_ref;
    // puts("\tA");fflush(stdout);
    for(int i = larrow_index + 1; i <= stop_index; i++){
        // printf("%s, %d\n", nian.charv + nian.token_indexes[i], tok_types.types[i]);
        if (tok_types.types[i] == VALUE || tok_types.types[i] == VARIABLE){
            if (strncmp(nian.charv + nian.token_indexes[i], "[", 3) == 0){
                i = getListClosingIndex(i);
            }
            count++;
        } else if (tok_types.types[i] == OPERATOR){
            if (strncmp(nian.charv + nian.token_indexes[i], "<-", 3) == 0){
                setNnpStr(nian.charv + nian.token_indexes[i - 1], &temp_str);
                temp_func_ref = getFromStorage(p_store, &temp_str);
                if (temp_func_ref->type != FUNC_OBJ){logMessage(FILE_PARSING, "Invalid function call on non-FuncObj\n"); exit(1);}
                if (temp_func_ref->func_type == USER_FUNC){puts("Have not implemented user-functions yet"); exit(1);}

                count -= temp_func_ref->num_args;
            } else {
                count--;
            }
        }
        // printf("Count: %d, num params: %d\n", count, num_params);
        if (count == num_params){
            if (i < stop_index && (tok_types.types[i + 1] == OPERATOR)){
                continue;
            }
            return i;
        }
    }
    // printf("NUM PARAMS %d", count);
    logMessage(FILE_PARSING, "Function Arguments not found\n");
    exit(1);
}

    // USER_FUNC,
    // TRUNCATE_M, STR_LEN_M, SUBSTRING_M, POP_M, FIND_M, INSERT_M, SET_M,
    // SPEAK_F, LISTEN_F, ASCII_TO_CH_F


void resolveFunction(ObjArray* p_obj_arr, int num_args){
    // printf("ARG: %d\nLEN: %u\n", num_args, p_obj_arr->length);fflush(stdout);
    // for (uint i = 0; i < p_obj_arr->length; i++){printf("\t[%u]: %p\n", i, p_obj_arr->objs[i]);}
    assert(p_obj_arr && p_obj_arr->capacity >= (uint)num_args + 1 && p_obj_arr->length >= (uint)num_args + 1 && p_obj_arr->objs);
    assert(num_args >= 0);
    FuncObj* func = p_obj_arr->objs[p_obj_arr->length - num_args - 1];
    enum PrebuiltFuncs_e function_type = func->func_type;
    
    bool is_method;
    switch (function_type){
        case TRUNCATE_M:
        case STR_LEN_M:
        case SUBSTRING_M:
        case POP_M:
        case FIND_M:
        case INSERT_M:
        case SET_M:
        is_method = true;
        break;
        default:
        is_method = false;
    }
    
    // puts("A");//fflush(stdout);
    // printf("\ttype: %d\n", *((Datatype_e*)(p_obj_arr->objs[0])));fflush(stdout);
    
    object_p method_obj = NULL;
    if (is_method){
        method_obj = p_obj_arr->objs[p_obj_arr->length - num_args - 2];
        // printf("\t%s\n", ((StrObj*)method_obj)->value.string.buffer);fflush(stdout);
    }
    
    // puts("B");fflush(stdout);
    // puts("\tA");fflush(stdout);
    object_p reted;
    switch(function_type){
    case USER_FUNC:
        logMessage(FILE_PARSING, "User function calls not implemented yet\n"); exit(1);
        break;
    case TRUNCATE_M:
        reted = getObjMethods(method_obj)->truncate(method_obj);
        break;
    case STR_LEN_M:
        reted = getObjMethods(method_obj)->str_len(method_obj);
        break;
    case SUBSTRING_M:
        reted = getObjMethods(method_obj)->substring(method_obj,
            p_obj_arr->objs[p_obj_arr->length - 2], p_obj_arr->objs[p_obj_arr->length - 1]);
        break;
    case POP_M:
        reted = getObjMethods(method_obj)->pop(method_obj,
            p_obj_arr->objs[p_obj_arr->length - 1]);
        break;
    case FIND_M:
        reted = getObjMethods(method_obj)->find(method_obj, p_obj_arr->objs[p_obj_arr->length - 1]);
        break;
    case INSERT_M:
        reted = getObjMethods(method_obj)->insert(method_obj,
            p_obj_arr->objs[p_obj_arr->length - 2], p_obj_arr->objs[p_obj_arr->length - 1]);
        break;
    case SET_M:
        reted = getObjMethods(method_obj)->set(method_obj,
            p_obj_arr->objs[p_obj_arr->length - 2], p_obj_arr->objs[p_obj_arr->length - 1]);
        break;
    case SPEAK_F:
    
    reted = speakFunction(p_obj_arr->objs[p_obj_arr->length - 1]);
        break;
    case LISTEN_F:
        reted = listenFunction();
        break;
    case ASCII_TO_CH_F:
        reted = asciiToChFunction(p_obj_arr->objs[p_obj_arr->length - 1]);
        break;
    default:
        logMessage(FILE_PARSING, "Unknown FuncObj func_type\n");
        exit(1);
    }

    // puts("\tB");fflush(stdout);


    for(int i = 0; i < num_args + 1; i++){
        popInObjArray(p_obj_arr, p_obj_arr->length - 1);
    }
    if (is_method && reted){
        freeObj(p_obj_arr->objs[p_obj_arr->length - 1]);
        p_obj_arr->objs[p_obj_arr->length - 1] = reted;
    } else if (reted) {
        appendInObjArray(p_obj_arr, reted);
        // p_obj_arr->objs[p_obj_arr->length] = reted;
    }

    // puts("\tC");fflush(stdout);

}