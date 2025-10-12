
#include "../../headers/datatypes.h"

static BoolObj* castBool(object_p op);
static DatatypeObj* castDatatype(__unused object_p op);
static ListObj* castList(object_p op);
static NumObj* castNum(object_p op);
static StrObj* castStr(object_p op);

static BoolObj* eqBool(object_p op1, BoolObj* op2);
static BoolObj* greaterthanBool(object_p op1, BoolObj* op2);
static BoolObj* lessthanBool(object_p op1, BoolObj* op2);
static BoolObj* noteqBool(object_p op1, BoolObj* op2);


OperatorsStruct boolOperators = {
    ._cast_bool = castBool,
    ._cast_datatype = castDatatype,
    ._cast_list = castList,
    ._cast_num = castNum,// 1 or 0
    ._cast_str = castStr,

    ._eq_bool = eqBool,
    ._greaterthan_bool = greaterthanBool,// true > false
    ._lessthan_bool = lessthanBool,// false < true
    ._noteq_bool = noteqBool
};

BuiltMethodsStruct boolMethods = BUILT_METHODS_STRUCT_NULL;


BoolObj* constructBoolObj(uint8_t value){
    logFuncStart(FUNCTION_CALLS, "start con BoolObj: %u\n", value);

    BoolObj* ret = malloc(sizeof(BoolObj));
    assert(ret && "Malloc construct BoolObj fail");

    ret->operators = &boolOperators;
    ret->methods = &boolMethods;
    ret->type = BOOL_OBJ;
    ret->value = value;

    logFuncEnds(FUNCTION_CALLS, "finish con BoolObj: %u\n", ret->value);
    return ret;
}





static BoolObj* castBool(object_p op){
    logFuncStart(FUNCTION_CALLS, "start cast BoolObj from BoolObj\n");
    BoolObj* ret = constructBoolObj(  ((BoolObj*)op)->value  );
    logFuncEnds(FUNCTION_CALLS, "finish cast BoolObj to BoolObj: %u\n", ret->value);
    return ret;
}

static DatatypeObj* castDatatype(__unused object_p op){
    logMessage(FUNCTION_CALLS, "Get Datatype of BoolObj\n");
    return constructDatatypeObj(BOOL_OBJ);
}

static ListObj* castList(object_p op){
    logFuncStart(FUNCTION_CALLS, "start cast ListObj from BoolObj\n");

    ObjArray temp = {
        .capacity = DEFAULT_OBJ_ARRAY_CAPACITY,
        .length = 1,
        .objs = malloc(sizeof(object_p) * DEFAULT_OBJ_ARRAY_CAPACITY)
    };
    assert(temp.objs && "cast BoolObj to ListObj");

    temp.objs[0] = copyObj(op);
    
    ListObj* ret = constructListObj(&temp);
    free(temp.objs[0]);
    free(temp.objs);

    logFuncEnds(FUNCTION_CALLS, "finish cast ListObj from BoolObj, value at index 0: %u\n", ( (BoolObj*)(ret->values.objs[0]) )->value );
    return ret;
}

static NumObj* castNum(object_p op){
    logFuncStart(FUNCTION_CALLS, "start cast NumObj from BoolObj\n");
    NumObj* ret = constructNumObj( ((BoolObj*)op)->value  > 0  );

    logFuncEnds(FUNCTION_CALLS, "finish cast NumObj from BoolObj: %lf\n", ( ret->value ) );
    return ret;
}

static StrObj* castStr(object_p op){
    logFuncStart(FUNCTION_CALLS, "start cast StrObj from BoolObj\n");
    StrObj* ret;
    if ( ((BoolObj*)op)->value == false ){
        ret = constructStrObj_char("False");
    } else {
        ret = constructStrObj_char("True");
    }
    assert(ret && "how?");

    if (ret->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        logFuncEnds(FUNCTION_CALLS, "finish cast StrObj from BoolObj: %s\n", ( ret->value.string.buffer ) );
    } else {
        logFuncEnds(FUNCTION_CALLS, "finish cast StrObj from BoolObj: %s\n", ( ret->value.string.dyn_str ) );
    }

    return ret;
}



static BoolObj* eqBool(object_p op1, BoolObj* op2){
    logFuncStart(FUNCTION_CALLS, "start equal BoolObj and BoolObj\n");
    BoolObj* ret = constructBoolObj( (((BoolObj*)op1)->value == 0) == (op2->value == 0)  );
    logFuncEnds(FUNCTION_CALLS, "finish equal BoolObj and BoolObj: %u\n", ret->value);
    return ret;
}

static BoolObj* greaterthanBool(object_p op1, BoolObj* op2){
    logFuncStart(FUNCTION_CALLS, "start greater than BoolObj and BoolObj\n");
    BoolObj* ret = constructBoolObj( ((BoolObj*)op1)->value != 0 && op2->value == 0 );
    logFuncEnds(FUNCTION_CALLS, "finish greater than BoolObj and BoolObj: %u\n", ret->value);
    return ret;
}
static BoolObj* lessthanBool(object_p op1, BoolObj* op2){
    logFuncStart(FUNCTION_CALLS, "start less than BoolObj and BoolObj\n");
    BoolObj* ret = constructBoolObj( ((BoolObj*)op1)->value == 0 && op2->value != 0 );
    logFuncEnds(FUNCTION_CALLS, "finish less than BoolObj and BoolObj: %u\n", ret->value);
    return ret;
}

static BoolObj* noteqBool(object_p op1, BoolObj* op2){
    logFuncStart(FUNCTION_CALLS, "start not equal BoolObj and BoolObj\n");
    BoolObj* ret = constructBoolObj( (((BoolObj*)op1)->value == 0) != (op2->value == 0)  );
    logFuncEnds(FUNCTION_CALLS, "finish not equal BoolObj and BoolObj: %u\n", ret->value);
    return ret;
}