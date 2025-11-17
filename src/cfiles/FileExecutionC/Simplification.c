#include "../../headers/datatypes.h"


extern Storage big_storage;

// int getFunctionFinalParameterIndex(int num_params, int larrow_index, int stop_index, const Storage* p_store){




void subCondenseObjsOperators(ObjArray* p_temp_stack, Datatype_e* datatype_arr, int start_index, int stop_index);

// functionCalled(p_temp_stack, &i, stop_index);
void functionCalled(ObjArray* p_obj_arr, int* p_curr_tok_index, int stop_index){

    assert(p_obj_arr && p_curr_tok_index);
    Datatype_e dat = *(Datatype_e*)(p_obj_arr->objs[p_obj_arr->length - 1]);
    if (dat != FUNC_OBJ){logMessage(FILE_PARSING, "Function called with a non-FuncObj\n"); exit(1);}

    extern Reader nian; extern TokenTyper tok_types;
    assert(nian.charv && nian.sz > 0 && nian.tok_ind_capacity > 1 && nian.token_indexes && nian.tok_ind_len > *p_curr_tok_index);
    assert(tok_types.size > 1 && tok_types.types);

    FuncObj* func_ref = p_obj_arr->objs[p_obj_arr->length - 1];

    if (func_ref->func_type == USER_FUNC){puts("Have not implemented user-defined functions yet"); exit(1);}

    // puts("A");fflush(stdout);

    int start_index = *p_curr_tok_index;
    int final_index = getFunctionFinalParameterIndex(func_ref->num_args, start_index, stop_index, &big_storage);

    // puts("\tB");fflush(stdout);

    if (final_index != -1){
        subCondenseObjsOperators(p_obj_arr, NULL, start_index + 1, final_index);
        *p_curr_tok_index = final_index;
    }

    // puts("\tA");fflush(stdout);

    resolveFunction(p_obj_arr, func_ref->num_args);

    // puts("B");fflush(stdout);
    // FUNCTION RESOLUTION

}






// static void generateListFromBrackets(ListObj* dst, ObjArray* p_temp_stack, int lbracket_index, int rbracket_index);



static void addValVarToTempStack(ObjArray* p_temp_stack, Datatype_e* datatype_arr, int index);
static void operationResolution(ObjArray* p_temp_stack, int final_index);





int getListClosingIndex(int open_bracket_index){
    assert(open_bracket_index >= 0);

    extern Reader nian; extern TokenTyper tok_types;

    assert(nian.charv && nian.sz > 0 && nian.tok_ind_capacity > 0 && nian.token_indexes && nian.tok_ind_len > open_bracket_index);
    assert(tok_types.size > 0 && tok_types.types);

    int bracket_nest = 1;
    for(int i = open_bracket_index + 1; i < nian.tok_ind_len; i++){
        if (strncmp(nian.charv + nian.token_indexes[i], "]", 2) == 0){
            bracket_nest--;
        } else if (strncmp(nian.charv + nian.token_indexes[i], "[", 2) == 0){
            bracket_nest++;
        }

        if (bracket_nest == 0) {
            return i;
        }
    }

    logMessage(FILE_PARSING, "\nList opening bracket, token[%d]: '%s', is not closed in the same line\n", open_bracket_index, nian.charv+nian.token_indexes[open_bracket_index]);
    exit(1);
}









void subCondenseObjsOperators(ObjArray* p_temp_stack, Datatype_e* datatype_arr, int start_index, int stop_index){
    // only start and stop on value/variables
    // printf("start: %d, stop: %d\n", start_index, stop_index);
    assert(start_index >= 0 && stop_index >= 0 && stop_index >= start_index);
    assert(p_temp_stack && p_temp_stack->objs != NULL && p_temp_stack->capacity > 0);
    // assert(datatype_arr);

    extern Reader nian; extern TokenTyper tok_types;

    assert(nian.charv && nian.sz > 0 && nian.tok_ind_capacity > 0 && nian.token_indexes && nian.tok_ind_len > stop_index);
    assert(tok_types.size > 0 && tok_types.types);

    // puts("\tB");fflush(stdout);


    // const char* curr_tok;
    int list_closed_at;
    
    for(int i = start_index; i <= stop_index; i++){
        // curr_tok = nian.charv + nian.token_indexes[i];
        // printf("[%d]\n", i);
        // puts("\tC");

        if (tok_types.types[i] == VALUE || tok_types.types[i] == VARIABLE){
            
            if (tok_types.types[i] == VALUE && strncmp("[", nian.charv + nian.token_indexes[i], 2) == 0){

                ObjArray temp = {.capacity = DEFAULT_OBJ_ARRAY_CAPACITY, .length = 0, .objs = NULL};
                ListObj* empty_list = constructListObj(&temp);
                appendInObjArray(p_temp_stack, empty_list);
                freeObj(empty_list);


                list_closed_at = getListClosingIndex(i);

                if (list_closed_at == i + 1){
                    i++; 
                    continue;
                }

                ListObj* added_list = p_temp_stack->objs[p_temp_stack->length - 1];
                subCondenseObjsOperators(&(added_list->values), NULL, i + 1, getListClosingIndex(i) - 1);
                i = list_closed_at;
                continue;
            }

            addValVarToTempStack(p_temp_stack, datatype_arr, i);

            
        } else if (tok_types.types[i] == OPERATOR){
            
            if (strncmp(nian.charv + nian.token_indexes[i], "<-", 3) == 0){

                functionCalled(p_temp_stack, &i, stop_index);

            } else if (strncmp(nian.charv + nian.token_indexes[i], "->", 3) == 0){
                assert(nian.tok_ind_len > i + 1 && tok_types.types[i+1] == VARIABLE);
                NnpStr var_identifier = makeNnpStr(nian.charv + nian.token_indexes[i+1]);
                assignVar(&big_storage, p_temp_stack, &var_identifier);
                freeNnpStr(&var_identifier);
            } else if (i + 1 < nian.tok_ind_len && nian.tok_ind_len >= 3 && (tok_types.types[i+1] == VALUE || tok_types.types[i+1] == VARIABLE)){
                i++;



                if (tok_types.types[i] == VALUE && strncmp("[", nian.charv + nian.token_indexes[i], 2) == 0){

                    ObjArray temp = {.capacity = DEFAULT_OBJ_ARRAY_CAPACITY, .length = 0, .objs = NULL};
                    ListObj* empty_list = constructListObj(&temp);
                    appendInObjArray(p_temp_stack, empty_list);
                    freeObj(empty_list);


                    list_closed_at = getListClosingIndex(i);

                    if (list_closed_at == i + 1){
                        operationResolution(p_temp_stack, i);
                        i++; 
                        continue;
                    }

                    ListObj* added_list = p_temp_stack->objs[p_temp_stack->length - 1];
                    subCondenseObjsOperators(&(added_list->values), NULL, i + 1, getListClosingIndex(i) - 1);
                    operationResolution(p_temp_stack, i);
                    i = list_closed_at;
                    continue;
                }







                addValVarToTempStack(p_temp_stack, datatype_arr, i);
                // if (strncmp(nian.charv + nian.token_indexes[i], "]", 2) == 0) continue;

                operationResolution(p_temp_stack, i);


            } else {
                logMessage(FILE_PARSING, "invalid operator syntax\n");
                puts("Invalid syntax");
                exit(1);
            }
        }
        
    }
}












void condenseObjsAndOperators(ObjArray* obj_line_stack){
    assert(obj_line_stack && obj_line_stack->length == 0);

    extern Reader nian;
    extern TokenTyper tok_types;

    assert(nian.charv && nian.sz > 0 && nian.tok_ind_capacity > 0 && nian.token_indexes);
    assert(tok_types.size > 0 && tok_types.types);

    
    if (nian.tok_ind_len == 0) return;

    // ObjArray temp_stack = {
    //     .capacity = nian.tok_ind_len,
    //     .length = 0,
    //     .objs = myMalloc(sizeof(object_p) * nian.tok_ind_len)
    // };
    // assert(temp_stack.objs);
    if (obj_line_stack->objs == NULL){
        obj_line_stack->capacity = 16;
        obj_line_stack->objs = myMalloc(sizeof(object_p) * obj_line_stack->capacity);
        assert(obj_line_stack->objs);
    }


    Datatype_e temp_stack_types[nian.tok_ind_len];

    subCondenseObjsOperators(obj_line_stack, temp_stack_types, 0, nian.tok_ind_len - 1);
}














static void addValVarToTempStack(ObjArray* p_temp_stack, Datatype_e* datatype_arr, int index){
    assert(p_temp_stack);
    // assert(p_temp_stack && datatype_arr);
    extern Reader nian;    extern TokenTyper tok_types;
    assert(nian.charv && nian.sz > 0 && nian.tok_ind_capacity > 0 && nian.token_indexes);
    assert(tok_types.size > 0 && tok_types.types);
    assert(nian.tok_ind_len > index && index >= 0);
    if (nian.tok_ind_len == 0) return;

    // printf("\t%d, \n", index);fflush(stdout);
    

    if (tok_types.types[index] == VALUE){

        if (datatype_arr){
            datatype_arr[p_temp_stack->length] = findValueType(nian.charv + nian.token_indexes[index]);
        }

        object_p obj = constructFromValue(index);

        assert(obj);
        appendInObjArray(p_temp_stack, obj);
        freeObj(obj);
        
    } else if (tok_types.types[index] == VARIABLE){
        // printf("DEBUG: %d\n", index);

        NnpStr tmp_str = makeNnpStr(nian.charv + nian.token_indexes[index]);
        object_p obj = getFromStorage(&big_storage, &tmp_str);
        assert(obj && "unidentified variable");

        if (datatype_arr){
            datatype_arr[p_temp_stack->length] = *((Datatype_e*)obj);
        }

        appendInObjArray(p_temp_stack, obj);
        // if (p_temp_stack->capacity == p_temp_stack->length){
        //     p_temp_stack->capacity *= 2;
        //     p_temp_stack->objs = myRealloc( p_temp_stack->objs, sizeof(object_p) * p_temp_stack->capacity);
        //     assert(p_temp_stack->objs && "grow obj arr fail");
        // }

        // p_temp_stack->objs[ p_temp_stack->length ] = obj;
        // p_temp_stack->length += 1;



        freeNnpStr(&tmp_str);

        // printf("DEBUG 2: %d\n", index);
    } else {
        logMessage(FILE_PARSING, "Misread addValVarToTempStack\n");
        puts("Misread addValVarToTempStack");
        exit(1);
    }
}

static OperatorsStruct* getObjOperators(object_p obj){
    Datatype_e dat = *((Datatype_e*)obj);
    switch (dat){
    case BOOL_OBJ:
    case DATATYPE_OBJ:
    case LIST_OBJ:
    case NUM_OBJ:
    case STR_OBJ:
        return ((NumObj*)obj)->operators;
    default:
        puts("Attempt at operating on object that has no operators");
        exit(1);
    }
}

BuiltMethodsStruct* getObjMethods(object_p obj){
    Datatype_e dat = *((Datatype_e*)obj);
    switch (dat){
    case BOOL_OBJ:
    case DATATYPE_OBJ:
    case LIST_OBJ:
    case NUM_OBJ:
    case STR_OBJ:
        return ((NumObj*)obj)->methods;
    default:
        puts("Attempt at using methods on object that has no methods");
        exit(1);
    }
}

static void applyOperation(ObjArray* p_temp_stack, object_p resolved_obj){
    popInObjArray(p_temp_stack, p_temp_stack->length - 1);
    freeObj(p_temp_stack->objs[p_temp_stack->length - 1]);
    p_temp_stack->objs[p_temp_stack->length - 1] = resolved_obj;
}



static void operationResolution(ObjArray* p_temp_stack, int final_index){
    assert(p_temp_stack && final_index >= 2 && p_temp_stack->objs && p_temp_stack->length >= 2 && p_temp_stack->capacity >= 2);
    extern Reader nian;    extern TokenTyper tok_types;
    assert(nian.charv && nian.sz > 0 && nian.tok_ind_capacity >= 2 && nian.token_indexes);
    assert(tok_types.size >= 2 && tok_types.types);
    assert(nian.tok_ind_len > final_index);
    if (nian.tok_ind_len == 0) return;

    // const int op1_tok_ind = final_index - 2, op2_tok_ind = final_index,
    const int operat_tok_ind = final_index - 1;
    // assert(
    //     (tok_types.types[op1_tok_ind] == VALUE || tok_types.types[op1_tok_ind] == VARIABLE)
    //     && (tok_types.types[operat_tok_ind] == OPERATOR)
    //     && (tok_types.types[op2_tok_ind] == VALUE || tok_types.types[op2_tok_ind] == VARIABLE)
    // );


    object_p op1 = p_temp_stack->objs[p_temp_stack->length - 2];
    object_p op2 = p_temp_stack->objs[p_temp_stack->length - 1];

    const char* operator_str = nian.charv + nian.token_indexes[operat_tok_ind];
    Datatype_e op2_type = *((Datatype_e*)(p_temp_stack->objs[p_temp_stack->length - 1]));
// NUM_OBJ, BOOL_OBJ, STR_OBJ, LIST_OBJ, DATATYPE_OBJ, INSTANCE_OBJ, FUNC_OBJ, CLASS_OBJ, NAO

// casting: val ~-> datatype



    if (strncmp(operator_str, "~->", 4) == 0){
        if (op2_type != DATATYPE_OBJ) {logMessage(FILE_PARSING, "invalid cast syntax\n");exit(1);}
        object_p resolved_obj;
        switch (((DatatypeObj*)op2)->value){
        case NUM_OBJ:{
            if (getObjOperators(op1)->_cast_num == NULL) {logMessage(FILE_PARSING, "invalid cast typing\n");exit(1);}
            resolved_obj = getObjOperators(op1)->_cast_num(op1);
            break;}
        case BOOL_OBJ:{
            if (getObjOperators(op1)->_cast_bool == NULL) {logMessage(FILE_PARSING, "invalid cast typing\n");exit(1);}
            resolved_obj = getObjOperators(op1)->_cast_bool(op1);
            break;}
        case STR_OBJ:{
            if (getObjOperators(op1)->_cast_str == NULL) {logMessage(FILE_PARSING, "invalid cast typing\n");exit(1);}
            resolved_obj = getObjOperators(op1)->_cast_str(op1);
            break;}
        case LIST_OBJ:{
            if (getObjOperators(op1)->_cast_list == NULL) {logMessage(FILE_PARSING, "invalid cast typing\n");exit(1);}
            resolved_obj = getObjOperators(op1)->_cast_list(op1);
            break;}
        case DATATYPE_OBJ:{
            if (getObjOperators(op1)->_cast_datatype == NULL) {logMessage(FILE_PARSING, "invalid cast typing\n");exit(1);}
            resolved_obj = getObjOperators(op1)->_cast_datatype(op1);
            break;}
        
        default:
            logMessage(FILE_PARSING, "unfound op2 cast type\n");
            exit(1);
        }
        assert(resolved_obj);
        applyOperation(p_temp_stack, resolved_obj);

    } else if (strncmp(operator_str, "+", 2) == 0){

        switch(op2_type){
        case NUM_OBJ:{
            if (getObjOperators(op1)->_add_num == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}

            
            object_p resolved_obj = getObjOperators(op1)->_add_num(op1, (NumObj*)op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);

            
            break;}
        case BOOL_OBJ:{
            if (getObjOperators(op1)->_add_bool == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_add_bool(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case STR_OBJ:{
            if (getObjOperators(op1)->_add_str == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_add_str(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case LIST_OBJ:{
            if (getObjOperators(op1)->_add_list == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_add_list(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case DATATYPE_OBJ:{
            if (getObjOperators(op1)->_add_datatype == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_add_datatype(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        default:
            logMessage(FILE_PARSING, "unknown op2 datatype\n");
            exit(1);
        }
    } else if (strncmp(operator_str, "~+", 3) == 0){
        switch(op2_type){
        case NUM_OBJ:{
            if (getObjOperators(op1)->_sub_num == NULL) {logMessage(FILE_PARSING, "invalid ~+ operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_sub_num(op1, (NumObj*)op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case BOOL_OBJ:{
            if (getObjOperators(op1)->_sub_bool == NULL) {logMessage(FILE_PARSING, "invalid ~+ operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_sub_bool(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case STR_OBJ:{
            if (getObjOperators(op1)->_sub_str == NULL) {logMessage(FILE_PARSING, "invalid ~+ operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_sub_str(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case LIST_OBJ:{
            if (getObjOperators(op1)->_sub_list == NULL) {logMessage(FILE_PARSING, "invalid ~+ operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_sub_list(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case DATATYPE_OBJ:{
            if (getObjOperators(op1)->_sub_datatype == NULL) {logMessage(FILE_PARSING, "invalid ~+ operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_sub_datatype(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        default:
            logMessage(FILE_PARSING, "unknown op2 datatype\n");
            exit(1);
        }
        
    } else if (strncmp(operator_str, "*", 2) == 0){
        switch(op2_type){
        case NUM_OBJ:{
            if (getObjOperators(op1)->_mult_num == NULL) {logMessage(FILE_PARSING, "invalid * operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_mult_num(op1, (NumObj*)op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case BOOL_OBJ:{
            if (getObjOperators(op1)->_mult_bool == NULL) {logMessage(FILE_PARSING, "invalid * operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_mult_bool(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case STR_OBJ:{
            if (getObjOperators(op1)->_mult_str == NULL) {logMessage(FILE_PARSING, "invalid * operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_mult_str(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case LIST_OBJ:{
            if (getObjOperators(op1)->_mult_list == NULL) {logMessage(FILE_PARSING, "invalid * operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_mult_list(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case DATATYPE_OBJ:{
            if (getObjOperators(op1)->_mult_datatype == NULL) {logMessage(FILE_PARSING, "invalid * operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_mult_datatype(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        default:
            logMessage(FILE_PARSING, "unknown op2 datatype\n");
            exit(1);
        }
        
    } else if (strncmp(operator_str, "~*", 3) == 0){
        switch(op2_type){
        case NUM_OBJ:{
            if (getObjOperators(op1)->_div_num == NULL) {logMessage(FILE_PARSING, "invalid ~* operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_div_num(op1, (NumObj*)op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case BOOL_OBJ:{
            if (getObjOperators(op1)->_div_bool == NULL) {logMessage(FILE_PARSING, "invalid ~* operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_div_bool(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case STR_OBJ:{
            if (getObjOperators(op1)->_div_str == NULL) {logMessage(FILE_PARSING, "invalid ~* operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_div_str(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case LIST_OBJ:{
            if (getObjOperators(op1)->_div_list == NULL) {logMessage(FILE_PARSING, "invalid ~* operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_div_list(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case DATATYPE_OBJ:{
            if (getObjOperators(op1)->_div_datatype == NULL) {logMessage(FILE_PARSING, "invalid ~* operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_div_datatype(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        default:
            logMessage(FILE_PARSING, "unknown op2 datatype\n");
            exit(1);
        }
        
    } else if (strncmp(operator_str, "=", 2) == 0){
        switch(op2_type){
        case NUM_OBJ:{
            if (getObjOperators(op1)->_eq_num == NULL) {logMessage(FILE_PARSING, "invalid = operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_eq_num(op1, (NumObj*)op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case BOOL_OBJ:{
            if (getObjOperators(op1)->_eq_bool == NULL) {logMessage(FILE_PARSING, "invalid = operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_eq_bool(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case STR_OBJ:{
            if (getObjOperators(op1)->_eq_str == NULL) {logMessage(FILE_PARSING, "invalid = operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_eq_str(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case LIST_OBJ:{
            if (getObjOperators(op1)->_eq_list == NULL) {logMessage(FILE_PARSING, "invalid = operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_eq_list(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case DATATYPE_OBJ:{
            if (getObjOperators(op1)->_eq_datatype == NULL) {logMessage(FILE_PARSING, "invalid = operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_eq_datatype(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        default:
            logMessage(FILE_PARSING, "unknown op2 datatype\n");
            exit(1);
        }
        
    } else if (strncmp(operator_str, "~=", 3) == 0){
        switch(op2_type){
        case NUM_OBJ:{
            if (getObjOperators(op1)->_noteq_num == NULL) {logMessage(FILE_PARSING, "invalid ~= operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_noteq_num(op1, (NumObj*)op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case BOOL_OBJ:{
            if (getObjOperators(op1)->_noteq_bool == NULL) {logMessage(FILE_PARSING, "invalid ~= operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_noteq_bool(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case STR_OBJ:{
            if (getObjOperators(op1)->_noteq_str == NULL) {logMessage(FILE_PARSING, "invalid ~= operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_noteq_str(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case LIST_OBJ:{
            if (getObjOperators(op1)->_noteq_list == NULL) {logMessage(FILE_PARSING, "invalid ~= operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_noteq_list(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case DATATYPE_OBJ:{
            if (getObjOperators(op1)->_noteq_datatype == NULL) {logMessage(FILE_PARSING, "invalid ~= operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_noteq_datatype(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        default:
            logMessage(FILE_PARSING, "unknown op2 datatype\n");
            exit(1);
        }
        
    } else if (strncmp(operator_str, ">", 2) == 0){
        switch(op2_type){
        case NUM_OBJ:{
            if (getObjOperators(op1)->_greaterthan_num == NULL) {logMessage(FILE_PARSING, "invalid > operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_greaterthan_num(op1, (NumObj*)op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case BOOL_OBJ:{
            if (getObjOperators(op1)->_greaterthan_bool == NULL) {logMessage(FILE_PARSING, "invalid > operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_greaterthan_bool(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case STR_OBJ:{
            if (getObjOperators(op1)->_greaterthan_str == NULL) {logMessage(FILE_PARSING, "invalid > operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_greaterthan_str(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case LIST_OBJ:{
            if (getObjOperators(op1)->_greaterthan_list == NULL) {logMessage(FILE_PARSING, "invalid > operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_greaterthan_list(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case DATATYPE_OBJ:{
            if (getObjOperators(op1)->_greaterthan_datatype == NULL) {logMessage(FILE_PARSING, "invalid > operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_greaterthan_datatype(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        default:
            logMessage(FILE_PARSING, "unknown op2 datatype\n");
            exit(1);
        }
        
    } else if (strncmp(operator_str, "<", 2) == 0){
        switch(op2_type){
        case NUM_OBJ:{
            if (getObjOperators(op1)->_lessthan_num == NULL) {logMessage(FILE_PARSING, "invalid < operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_lessthan_num(op1, (NumObj*)op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case BOOL_OBJ:{
            if (getObjOperators(op1)->_lessthan_bool == NULL) {logMessage(FILE_PARSING, "invalid < operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_lessthan_bool(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case STR_OBJ:{
            if (getObjOperators(op1)->_lessthan_str == NULL) {logMessage(FILE_PARSING, "invalid < operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_lessthan_str(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case LIST_OBJ:{
            if (getObjOperators(op1)->_lessthan_list == NULL) {logMessage(FILE_PARSING, "invalid < operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_lessthan_list(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case DATATYPE_OBJ:{
            if (getObjOperators(op1)->_lessthan_datatype == NULL) {logMessage(FILE_PARSING, "invalid < operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_lessthan_datatype(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        default:
            logMessage(FILE_PARSING, "unknown op2 datatype\n");
            exit(1);
        }
        
    } else if (strncmp(operator_str, ".", 2) == 0){
        if (*((Datatype_e*)op1) != LIST_OBJ || op2_type != NUM_OBJ) {logMessage(FILE_PARSING, "invalid . operation\n");exit(1);}
        object_p resolved_obj = getObjMethods(op1)->at_loc((ListObj*)op1, (NumObj*)op2);
        assert(resolved_obj);
        applyOperation(p_temp_stack, resolved_obj);
    } else {
        puts("Operator found then not found???");
        exit(1);
    }
}



object_p constructFromValue(int tok_index){


    extern Reader nian;
    extern TokenTyper tok_types;

    assert(nian.charv && nian.sz > 0 && nian.tok_ind_capacity > 0 && nian.token_indexes);
    assert(tok_types.size > 0 && tok_types.types);
    assert(nian.tok_ind_len > tok_index);

    char* tok = nian.charv + nian.token_indexes[tok_index];
    Datatype_e dat = findValueType(tok);
    switch (dat){
    case NUM_OBJ:
        return constructNumObj(strtod(tok, NULL));
    case BOOL_OBJ:
        if (strncmp(tok, "True", 5) == 0){
            return constructBoolObj(true);
        } else if (strncmp(tok, "False", 6) == 0){
            return constructBoolObj(false);
        }
        logMessage(FILE_PARSING, "Attempt to make bool of value neither True nor False\n");
        exit(1);
    case DATATYPE_OBJ:
        if (strncmp("BoolObj", tok, 8) == 0){
            return constructDatatypeObj(BOOL_OBJ);
        } else if (strncmp("DatatypeObj", tok, 12) == 0){
            return constructDatatypeObj(DATATYPE_OBJ);
        } else if (strncmp("ListObj", tok, 8) == 0){
            return constructDatatypeObj(LIST_OBJ);
        } else if (strncmp("NumObj", tok, 7) == 0){
            return constructDatatypeObj(NUM_OBJ);
        } else if (strncmp("StrObj", tok, 7) == 0){
            return constructDatatypeObj(STR_OBJ);
        }
        logMessage(FILE_PARSING, "Attempt to make datatype of invalid value\n");
        exit(1);
    case STR_OBJ:{
        NnpStr tmp_str = makeNnpStr(tok + 1);
        size_t le;
        StrObj* ret;
        if (tmp_str.union_mode == NNPSTR_UNIONMODE_BUFFER){
            le = strnlen(tmp_str.string.buffer, STRING_U_BUFFER_SIZE);
            if (le >= 1) tmp_str.string.buffer[le - 1] = '\0';
            ret = constructStrObj_char(tmp_str.string.buffer);
        } else {
            le = strlen(tmp_str.string.dyn_str);
            if (le >= 1) tmp_str.string.dyn_str[le - 1] = '\0';
            ret = constructStrObj_char(tmp_str.string.dyn_str);
            freeNnpStr(&tmp_str);
        }
        return ret;}
    // case LIST_OBJ:{
    //     if (strncmp(tok, "]", 2) == 0) return NULL;
    //     ObjArray empty = {.capacity = DEFAULT_OBJ_ARRAY_CAPACITY + 1, .length = 0, .objs = NULL};
    //     return constructListObj(&empty);}
    default:
        logMessage(FILE_PARSING, "Attempted to construct from an invalid value\n");
        exit(1);
    }

}


