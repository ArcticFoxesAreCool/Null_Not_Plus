#include "../../headers/datatypes.h"



Storage big_storage = {
    .capacity = 0,
    .length = 0,
    .identifiers = NULL,
    .objs = NULL
};



void initBigStorage(int size){
    assert(size > 0 && big_storage.identifiers == NULL && big_storage.capacity == 0 && big_storage.length == 0 && big_storage.objs == NULL);
    big_storage.objs = malloc(sizeof(object_p) * size);
    big_storage.identifiers = malloc(sizeof(NnpStr) * size);
    assert(big_storage.objs && big_storage.identifiers);

    big_storage.capacity = size;
    big_storage.length = 0;
}


void deepFreeBigStorage(){
    assert(big_storage.objs && big_storage.identifiers && big_storage.capacity > 0);

    for(uint i = 0; i < big_storage.length; i++){
        freeObj(big_storage.objs[i]);
        freeNnpStr(  &(big_storage.identifiers[i])  );
    }

    free(big_storage.objs);
    free(big_storage.identifiers);

    big_storage.capacity = 0;
    big_storage.length = 0;
}






static void addValVarToTempStack(ObjArray* p_temp_stack, Datatype_e* datatype_arr, int index);
static void operationResolution(ObjArray* p_temp_stack, int final_index);




void condenseObjsAndOperators(ObjArray* p_empty_objarr){
    assert(p_empty_objarr && p_empty_objarr->objs == NULL && p_empty_objarr->capacity == 0 && p_empty_objarr->length == 0);

    extern Reader nian;
    extern TokenTyper tok_types;

    assert(nian.charv && nian.sz > 0 && nian.tok_ind_capacity > 0 && nian.token_indexes);
    assert(tok_types.size > 0 && tok_types.types);
    
    if (nian.tok_ind_len == 0) return;

    ObjArray temp_stack = {
        .capacity = nian.tok_ind_len,
        .length = 0,
        .objs = malloc(sizeof(object_p) * nian.tok_ind_len)
    };
    assert(temp_stack.objs);


    Datatype_e temp_stack_types[nian.tok_ind_len];


    for (int i = 0; i < nian.tok_ind_len; i++){
        if (strncmp(nian.charv + nian.token_indexes[i], "->", 3) == 0) break;
        
        if (tok_types.types[i] == VALUE || tok_types.types[i] == VARIABLE){
            
            addValVarToTempStack(&temp_stack, temp_stack_types, i);
            
        } else if (tok_types.types[i] == OPERATOR){
            if (i + 1 < nian.tok_ind_len && nian.tok_ind_len >= 3 && (tok_types.types[i+1] == VALUE || tok_types.types[i+1] == VARIABLE) && (tok_types.types[i-1] == VALUE || tok_types.types[i-1] == VARIABLE)){
                i++;

                addValVarToTempStack(&temp_stack, temp_stack_types, i);
                operationResolution(&temp_stack, i);


            } else {
                logMessage(FILE_PARSING, "invalid operator syntax\n");
                puts("Invalid syntax");
                exit(1);
            }
        }
       
    }

    printf("TEMPORTY CODE HERE\n"); fflush(stdout);
    char val_to_str[512];
    for(uint i = 0; i < temp_stack.length; i++){
        objValtoStr(val_to_str, temp_stack.objs[i]);
        printf("\t%s\n", val_to_str);
    }

    freeObjArrayEntries(&temp_stack);
    free(temp_stack.objs);

}






static void addValVarToTempStack(ObjArray* p_temp_stack, Datatype_e* datatype_arr, int index){
    assert(p_temp_stack && datatype_arr);
    extern Reader nian;    extern TokenTyper tok_types;
    assert(nian.charv && nian.sz > 0 && nian.tok_ind_capacity > 0 && nian.token_indexes);
    assert(tok_types.size > 0 && tok_types.types);
    assert(nian.tok_ind_len > index && index >= 0);
    if (nian.tok_ind_len == 0) return;


    if (tok_types.types[index] == VALUE){

        datatype_arr[p_temp_stack->length] = findValueType(nian.charv + nian.token_indexes[index]);
        object_p obj = constructFromValue(index);
        assert(obj);
        appendInObjArray(p_temp_stack, obj);

    } else if (tok_types.types[index] == VARIABLE){
        printf("DEBUG: %d\n", index);

        NnpStr tmp_str;
        setNnpStr(nian.charv + nian.token_indexes[index], &tmp_str);
        object_p obj = getFromStorage(&big_storage, &tmp_str);
        assert(obj && "unidentified variable");
        datatype_arr[p_temp_stack->length] = *((Datatype_e*)obj);
        appendInObjArray(p_temp_stack, obj);
        freeNnpStr(&tmp_str);
    } else {
        logMessage(FILE_PARSING, "Misread addValVarToTempStack\n");
        puts("Misread addValVarToTempStack");
        exit(1);
    }
}

static OperatorsStruct* getObjOperators(object_p obj){
    return ((NumObj*)obj)->operators;
}

static BuiltMethodsStruct* getObjMethods(object_p obj){
    return ((NumObj*)obj)->methods;
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

    const int op1_tok_ind = final_index - 2, op2_tok_ind = final_index, operat_tok_ind = final_index - 1;
    assert(
        (tok_types.types[op1_tok_ind] == VALUE || tok_types.types[op1_tok_ind] == VARIABLE)
        && (tok_types.types[operat_tok_ind] == OPERATOR)
        && (tok_types.types[op2_tok_ind] == VALUE || tok_types.types[op2_tok_ind] == VARIABLE)
    );


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

            
            // puts("A");fflush(stdout);
            // object_p resolved_obj = ((NumObj*)op1)->operators->_add_num(op1, (NumObj*)op2);
            object_p resolved_obj = getObjOperators(op1)->_add_num(op1, (NumObj*)op2);
            // printf("%p, %p\n", op1, ((NumObj*)op1)->operators);
            // puts("B");fflush(stdout);
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
            if (getObjOperators(op1)->_sub_num == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_sub_num(op1, (NumObj*)op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case BOOL_OBJ:{
            if (getObjOperators(op1)->_sub_bool == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_sub_bool(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case STR_OBJ:{
            if (getObjOperators(op1)->_sub_str == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_sub_str(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case LIST_OBJ:{
            if (getObjOperators(op1)->_sub_list == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_sub_list(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case DATATYPE_OBJ:{
            if (getObjOperators(op1)->_sub_datatype == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
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
            if (getObjOperators(op1)->_mult_num == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_mult_num(op1, (NumObj*)op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case BOOL_OBJ:{
            if (getObjOperators(op1)->_mult_bool == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_mult_bool(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case STR_OBJ:{
            if (getObjOperators(op1)->_mult_str == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_mult_str(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case LIST_OBJ:{
            if (getObjOperators(op1)->_mult_list == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_mult_list(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case DATATYPE_OBJ:{
            if (getObjOperators(op1)->_mult_datatype == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
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
            if (getObjOperators(op1)->_div_num == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_div_num(op1, (NumObj*)op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case BOOL_OBJ:{
            if (getObjOperators(op1)->_div_bool == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_div_bool(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case STR_OBJ:{
            if (getObjOperators(op1)->_div_str == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_div_str(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case LIST_OBJ:{
            if (getObjOperators(op1)->_div_list == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_div_list(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case DATATYPE_OBJ:{
            if (getObjOperators(op1)->_div_datatype == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
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
            if (getObjOperators(op1)->_eq_num == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_eq_num(op1, (NumObj*)op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case BOOL_OBJ:{
            if (getObjOperators(op1)->_eq_bool == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_eq_bool(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case STR_OBJ:{
            if (getObjOperators(op1)->_eq_str == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_eq_str(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case LIST_OBJ:{
            if (getObjOperators(op1)->_eq_list == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_eq_list(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case DATATYPE_OBJ:{
            if (getObjOperators(op1)->_eq_datatype == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
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
            if (getObjOperators(op1)->_noteq_num == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_noteq_num(op1, (NumObj*)op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case BOOL_OBJ:{
            if (getObjOperators(op1)->_noteq_bool == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_noteq_bool(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case STR_OBJ:{
            if (getObjOperators(op1)->_noteq_str == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_noteq_str(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case LIST_OBJ:{
            if (getObjOperators(op1)->_noteq_list == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_noteq_list(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case DATATYPE_OBJ:{
            if (getObjOperators(op1)->_noteq_datatype == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
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
            if (getObjOperators(op1)->_greaterthan_num == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_greaterthan_num(op1, (NumObj*)op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case BOOL_OBJ:{
            if (getObjOperators(op1)->_greaterthan_bool == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_greaterthan_bool(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case STR_OBJ:{
            if (getObjOperators(op1)->_greaterthan_str == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_greaterthan_str(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case LIST_OBJ:{
            if (getObjOperators(op1)->_greaterthan_list == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_greaterthan_list(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case DATATYPE_OBJ:{
            if (getObjOperators(op1)->_greaterthan_datatype == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
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
            if (getObjOperators(op1)->_lessthan_num == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_lessthan_num(op1, (NumObj*)op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case BOOL_OBJ:{
            if (getObjOperators(op1)->_lessthan_bool == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_lessthan_bool(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case STR_OBJ:{
            if (getObjOperators(op1)->_lessthan_str == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_lessthan_str(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case LIST_OBJ:{
            if (getObjOperators(op1)->_lessthan_list == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
            object_p resolved_obj = getObjOperators(op1)->_lessthan_list(op1, op2);
            assert(resolved_obj);
            applyOperation(p_temp_stack, resolved_obj);
            break;}
        case DATATYPE_OBJ:{
            if (getObjOperators(op1)->_lessthan_datatype == NULL) {logMessage(FILE_PARSING, "invalid + operation\n");exit(1);}
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

    puts("must handle lists");

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
        NnpStr tmp_str;
        setNnpStr(tok + 1, &tmp_str);
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
    default:
        logMessage(FILE_PARSING, "Attempted to construct from an invalid value\n");
        exit(1);
    }

}
// if ( fabs( strtod(token, NULL) ) > 2 * __DBL_EPSILON__ || token[0] == '0'){
//         return NUM_OBJ;
//     } else if (strncmp(token, "True", 5) == 0 || strncmp(token, "False", 6) == 0){
//         return BOOL_OBJ;
//     } else if (isDattype(token)){
//         return DATATYPE_OBJ;
//     } else if (token[0] == '\"'){
//         return STR_OBJ;
//     } else if (strncmp("[", token, 2) == 0 || strncmp("]", token, 2) == 0){
//         return LIST_OBJ;


    // "->",// assignment and return
    // "~->",// typecast
    // "<-",// func and method call
    // "~<-",// class constructor 
    // "+",// add
    // "~+",// subtract
    // "*",// mult
    // "~*",// div
    // "=",// is equal to
    // "~=",// not equal
    // ">",// greater than
    // "<",// less than
    // "."// index and class var/method getting







/*
Plan:
steps:
condense,
    what should be left, function calls, keywords, and assignment operators
    contains 
    // find a value/var, continue to evaluate the operators that come next
*/



// void executeLine(){
//     // extern Reader nian;
//     // extern TokenTyper tok_types;

//     // int operator_count = 0;

// }
