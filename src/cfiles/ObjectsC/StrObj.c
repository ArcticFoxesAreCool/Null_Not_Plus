
#include "../../headers/datatypes.h"


static object_p addStr(object_p op1, StrObj* op2);
static object_p multNum(object_p op1, NumObj* op2);

static BoolObj* castBool(object_p op);
static DatatypeObj* castDatatype(__unused object_p op);
static ListObj* castList(object_p op);
static NumObj* castNum(object_p op);
static StrObj* castStr(object_p op);

static BoolObj* eqStr(object_p op1, StrObj* op2);
static BoolObj* noteqStr(object_p op1, StrObj* op2);







OperatorsStruct strOperators = {
    ._add_str = addStr,
    ._mult_num = multNum,
    ._cast_bool = castBool,
    ._cast_datatype = castDatatype,
    ._cast_list = castList,
    ._cast_num = castNum,
    ._cast_str = castStr,
    ._eq_str = eqStr,
    ._noteq_str = noteqStr
};




static StrObj* str_sub_string(StrObj* st, NumObj* start, NumObj* end);
static NumObj* str_length(StrObj* st);


BuiltMethodsStruct strMethods = {
    .str_len = str_length,
    .substring = str_sub_string
};


StrObj* constructStrObj_char(const char* nullEndedStr){
    logFuncStart(FUNCTION_CALLS, "start con StrObj: %s\n", nullEndedStr);
 
    assert(nullEndedStr);

    StrObj* ret = malloc(sizeof(StrObj));
    assert(ret && "Malloc a new str fail");

    ret->operators = &strOperators;
    ret->methods = &strMethods;
    ret->type = STR_OBJ;
    
    setNnpStr(nullEndedStr, &(ret->value) );

    if (ret->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        logFuncEnds(FUNCTION_CALLS, "finish con StrObj: %s\n", ret->value.string.buffer);
    } else {
        logFuncEnds(FUNCTION_CALLS, "finish con StrObj: %s\n", ret->value.string.dyn_str);
    }
    return ret;
    
}












static object_p addStr(object_p op1, StrObj* op2){
    assert(op1 && op2);

    logFuncStart(FUNCTION_CALLS, "start add StrObj to StrObj\n");
    
    uint op1_length;
    if (  ((StrObj*)op1)->value.union_mode == NNPSTR_UNIONMODE_BUFFER  ){
        op1_length = strnlen(  ((StrObj*)op1)->value.string.buffer, STRING_U_BUFFER_SIZE  );
    } else {
        op1_length = strlen(  ((StrObj*)op1)->value.string.dyn_str  );
    }

    uint op2_length;
    if (op2->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        op2_length = strnlen(op2->value.string.buffer, STRING_U_BUFFER_SIZE);
    } else {
        op2_length = strlen(op2->value.string.dyn_str);
    }


    char buffer[op1_length + op2_length + 1];
    
    if (  ((StrObj*)op1)->value.union_mode == NNPSTR_UNIONMODE_BUFFER  ){
        strncpy(buffer, ((StrObj*)op1)->value.string.buffer, op1_length + 1);
    } else {
        strncpy(buffer, ((StrObj*)op1)->value.string.dyn_str, op1_length + 1);
    }
    
    if (  op2->value.union_mode == NNPSTR_UNIONMODE_BUFFER  ){
        strncat(buffer, op2->value.string.buffer, op2_length);
    } else {
        strncat(buffer, op2->value.string.dyn_str, op2_length);
    }

    StrObj* ret = constructStrObj_char(buffer);

    if (ret->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        logFuncEnds(FUNCTION_CALLS, "finish add StrObj to StrObj: %s\n", ret->value.string.buffer);
    } else {
        logFuncEnds(FUNCTION_CALLS, "finish add StrObj to StrObj: %s\n", ret->value.string.dyn_str);
    }

    return (object_p)ret;
}




static object_p multNum(object_p op1, NumObj* op2){

    if ( ((StrObj*)op1)->value.union_mode == NNPSTR_UNIONMODE_BUFFER)
        logFuncStart(FUNCTION_CALLS, "start mult NumObj to StrObj (%s)\n", ((StrObj*)op1)->value.string.buffer);
    else
        logFuncStart(FUNCTION_CALLS, "start mult NumObj to StrObj (%s)\n", ((StrObj*)op1)->value.string.dyn_str);

    uint new_str_len;
    assert(op1 && op2 && "num mult str");
    
    
    if (((StrObj*)op1)->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        new_str_len = op2->value * strnlen( ((StrObj*)op1)->value.string.buffer, STRING_U_BUFFER_SIZE);
    } else {
        new_str_len = op2->value * strlen( ((StrObj*)op1)->value.string.dyn_str);
    }
    
    // assert(  op2->value > 2 * __DBL_EPSILON__  );
    assert( new_str_len >= 0  );

    char string[new_str_len + 1]; 
    string[0] = '\0';

    if (((StrObj*)op1)->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        for (uint i = 0; i < (uint)(op2->value); i++)
            strncat(string, ((StrObj*)op1)->value.string.buffer, STRING_U_BUFFER_SIZE);
    } else {
        for (uint i = 0; i < (uint)(op2->value); i++)
            strcat(string, ((StrObj*)op1)->value.string.dyn_str);
    }

    StrObj* ret = constructStrObj_char(string);

    if (ret->value.union_mode == NNPSTR_UNIONMODE_BUFFER)
        logFuncEnds(FUNCTION_CALLS, "finish mult NumObj to StrObj (%s)\n", ret->value.string.buffer);
    else
        logFuncEnds(FUNCTION_CALLS, "finish mult NumObj to StrObj (%s)\n", ret->value.string.dyn_str);

    return (object_p)ret;
}

static BoolObj* castBool(object_p op){
    logFuncStart(FUNCTION_CALLS, "start cast BoolObj from StrObj\n");

    int length;
    if (((StrObj*)op)->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        length = strnlen( ((StrObj*)op)->value.string.buffer, STRING_U_BUFFER_SIZE );
    }  else {
        length = strlen(  ((StrObj*)op)->value.string.dyn_str  );
    }

    BoolObj* ret = constructBoolObj(  length > 0  );

    logFuncEnds(FUNCTION_CALLS, "finish cast BoolObj from StrObj: %u\n", ret->value);
    return ret;
}

static DatatypeObj* castDatatype(__unused object_p op){
    logMessage(FUNCTION_CALLS, "Get Datatype of StrObj\n");
    return constructDatatypeObj(STR_OBJ);
}

static ListObj* castList(object_p op){
     logFuncStart(FUNCTION_CALLS, "start cast ListObj from StrObj\n");

    ObjArray temp = {
        .capacity = DEFAULT_OBJ_ARRAY_CAPACITY,
        .length = 1,
        .objs = malloc(sizeof(object_p) * DEFAULT_OBJ_ARRAY_CAPACITY)
    };
    temp.objs[0] = copyObj(op);
    
    ListObj* ret = constructListObj(&temp);
    free(temp.objs[0]);
    free(temp.objs);

    if ( ((StrObj*)(ret->values.objs[0]) )->value.union_mode == NNPSTR_UNIONMODE_BUFFER ){
        logFuncEnds(FUNCTION_CALLS, "finish cast ListObj from NumObj, value at index 0: %s\n", ( (StrObj*)(ret->values.objs[0]) )->value.string.buffer );
    } else {
        logFuncEnds(FUNCTION_CALLS, "finish cast ListObj from NumObj, value at index 0: %s\n", ( (StrObj*)(ret->values.objs[0]) )->value.string.dyn_str );
    }

    return ret;
}

static NumObj* castNum(object_p op){
    logFuncStart(FUNCTION_CALLS, "start cast NumObj from StrObj\n");

    double op_double_val;
    if (((StrObj*)op)->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        op_double_val =  atof(  ((StrObj*)op)->value.string.buffer );
    } else {
        op_double_val =  atof( ((StrObj*)op)->value.string.dyn_str );
    }

    NumObj* ret = constructNumObj(op_double_val);
    logFuncEnds(FUNCTION_CALLS, "finish cast NumObj from StrObj: %lf\n", ret->value);
    return ret;
}

static StrObj* castStr(object_p op){
    logFuncStart(FUNCTION_CALLS, "start cast StrObj from StrObj\n");
    StrObj* ret = copyObj(op);
    if (ret->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        logFuncEnds(FUNCTION_CALLS, "finish cast StrObj to StrObj: %lf\n", ret->value.string.buffer);
    } else {
        logFuncEnds(FUNCTION_CALLS, "finish cast StrObj to StrObj: %lf\n", ret->value.string.dyn_str);
    }

    return ret;
}

static BoolObj* eqStr(object_p op1, StrObj* op2){
    logFuncStart(FUNCTION_CALLS, "start equals StrObj to StrObj\n");

    if (  ((StrObj*)op1)->value.union_mode != op2->value.union_mode  ){
        BoolObj* ret = constructBoolObj(false);
        logFuncEnds(FUNCTION_CALLS, "finish equals StrObj to StrObj: %u\n", ret->value);
        return ret;
    }

    if (op2->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        BoolObj* ret = constructBoolObj(  strncmp( op2->value.string.buffer, ((StrObj*)op1)->value.string.buffer, STRING_U_BUFFER_SIZE ) == 0  );
        logFuncEnds(FUNCTION_CALLS, "finish equals StrObj to StrObj: %u\n", ret->value);
        return ret;
    }

    BoolObj* ret = constructBoolObj(  strcmp( op2->value.string.dyn_str, ((StrObj*)op1)->value.string.dyn_str) == 0  );
    logFuncEnds(FUNCTION_CALLS, "finish equals StrObj to StrObj: %u\n", ret->value);
    return ret;
}

static BoolObj* noteqStr(object_p op1, StrObj* op2){
    logFuncStart(FUNCTION_CALLS, "start not equals StrObj to StrObj\n");

    if (  ((StrObj*)op1)->value.union_mode != op2->value.union_mode  ){
        BoolObj* ret = constructBoolObj(true);
        logFuncEnds(FUNCTION_CALLS, "finish not equals StrObj to StrObj: %u\n", ret->value);
        return ret;
    }

    if (op2->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        BoolObj* ret = constructBoolObj(  strncmp( op2->value.string.buffer, ((StrObj*)op1)->value.string.buffer, STRING_U_BUFFER_SIZE ) != 0  );
        logFuncEnds(FUNCTION_CALLS, "finish not equals StrObj to StrObj: %u\n", ret->value);
        return ret;
    }

    BoolObj* ret = constructBoolObj(  strcmp( op2->value.string.dyn_str, ((StrObj*)op1)->value.string.dyn_str) != 0  );
    logFuncEnds(FUNCTION_CALLS, "finish not equals StrObj to StrObj: %u\n", ret->value);
    return ret;
}
























static StrObj* str_sub_string(StrObj* st, NumObj* start, NumObj* end){
    logFuncStart(FUNCTION_CALLS, "start sub_string method StrObj\n");


    int st_len;
    if (st->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        st_len = strnlen(st->value.string.buffer, STRING_U_BUFFER_SIZE);
    } else {
        st_len = strlen(st->value.string.dyn_str);
    }

    assert(end->value >= start->value && start->value >= 0 && (int)trunc(end->value) < st_len);
    // bounds check

    size_t start_index = (size_t)trunc(start->value);
    size_t end_index = (size_t)trunc(end->value);
    size_t chars_to_copy = end_index - start_index + 1;

    // "abcde"
    // start=1, end=3
    char buffer[chars_to_copy + 1];
    buffer[0] = '\0';
    if (st->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        strncpy(buffer, ((st->value.string.buffer)+start_index), chars_to_copy);
    } else {
        strncpy(buffer, ((st->value.string.dyn_str)+start_index), chars_to_copy);
    }
    buffer[chars_to_copy] = '\0';

    StrObj* ret = constructStrObj_char(buffer);


    if (ret->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        logFuncStart(FUNCTION_CALLS, "start sub_string method StrObj: %s\n", ret->value.string.buffer);
    } else {
        logFuncStart(FUNCTION_CALLS, "start sub_string method StrObj: %s\n", ret->value.string.dyn_str);
    }
    return ret;
}





static NumObj* str_length(StrObj* st){
    logFuncStart(FUNCTION_CALLS, "start str_length method StrObj\n");

    NumObj* ret;
    if (st->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        ret = constructNumObj(  (double)strnlen(st->value.string.buffer, STRING_U_BUFFER_SIZE)  );
    } else {
        ret = constructNumObj(  (double)strlen(st->value.string.dyn_str)  );
    }

    logFuncEnds(FUNCTION_CALLS, "finish str_length method StrObj: %lf\n", ret->value);
    return ret;
}