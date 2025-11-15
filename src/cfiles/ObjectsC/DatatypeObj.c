
#include "../../headers/datatypes.h"



static DatatypeObj* castDatatype(object_p op);
static ListObj* castList(object_p op);
static StrObj* castStr(object_p op);
static BoolObj* eqDatatype(object_p op1, DatatypeObj* op2);
static BoolObj* noteqDatatype(object_p op1, DatatypeObj* op2);


OperatorsStruct datatypeOperators = {
    ._cast_datatype = castDatatype,
    ._cast_list = castList,
    ._cast_str = castStr,

    ._eq_datatype = eqDatatype,
    ._noteq_datatype = noteqDatatype
};

BuiltMethodsStruct datatypeMethods = BUILT_METHODS_STRUCT_NULL;


DatatypeObj* constructDatatypeObj(Datatype_e value){
    logFuncStart(FUNCTION_CALLS, "start con DatatypeObj: %d\n", value);

    DatatypeObj* ret = myMalloc(sizeof(DatatypeObj));
    assert(ret && "Malloc a new datatype fail");

    ret->operators = &datatypeOperators;
    ret->methods = &datatypeMethods;
    ret->type = DATATYPE_OBJ;
    ret->value = value;

    logFuncEnds(FUNCTION_CALLS, "finish con DatatypeObj: %d\n", ret->value);
    return ret;
}




static DatatypeObj* castDatatype(object_p op){
    logFuncStart(FUNCTION_CALLS, "start cast DatatypeObj from DatatypeObj\n");
    DatatypeObj* ret = constructDatatypeObj( ((DatatypeObj*)op)->value );
    logFuncEnds(FUNCTION_CALLS, "finish cast DatatypeObj to DatatypeObj: %u\n", ret->value);
    return ret;
}









static ListObj* castList(object_p op){
    logFuncStart(FUNCTION_CALLS, "start cast ListObj from DatatypeObj\n");
    ObjArray temp = {
        .capacity = DEFAULT_OBJ_ARRAY_CAPACITY,
        .length = 1,
        .objs = myMalloc(sizeof(object_p) * DEFAULT_OBJ_ARRAY_CAPACITY)
    };
    assert(temp.objs && "cast DatatypeObj to ListObj");

    temp.objs[0] = copyObj(op);
    
    ListObj* ret = constructListObj(&temp);
    myFree(temp.objs[0]);
    myFree(temp.objs);

    logFuncEnds(FUNCTION_CALLS, "finish cast ListObj from DatatypeObj, value at index 0: %u\n", ( (DatatypeObj*)(ret->values.objs[0]) )->value );
    return ret;
}

static StrObj* castStr(object_p op){
    logFuncStart(FUNCTION_CALLS, "start cast StrObj from DatatypeObj\n");
    StrObj* ret;
    Datatype_e dat = ((DatatypeObj*)op)->value;
    
    switch (dat){
    case NUM_OBJ:
        ret = constructStrObj_char("NumObj");
        break;
    case BOOL_OBJ:
        ret = constructStrObj_char("BoolObj");
        break;
    case LIST_OBJ:
        ret = constructStrObj_char("ListObj");
        break;
    case STR_OBJ:
        ret = constructStrObj_char("StrObj");
        break;
    case DATATYPE_OBJ:
        ret = constructStrObj_char("DatatypeObj");
        break;
    case INSTANCE_OBJ:
        ret = constructStrObj_char("InstanceObj");
        break;
    case CLASS_OBJ:
        ret = constructStrObj_char("ClassObj");
        break;
    case FUNC_OBJ:
        ret = constructStrObj_char("FuncObj");
        break;
    default:
        puts("Datatype of DatatypeObj cast to Str not found");
        exit(1);
    }



    assert(ret && "how?");

    if (ret->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        logFuncEnds(FUNCTION_CALLS, "finish cast StrObj from DatatypeObj: %s\n", ( ret->value.string.buffer ) );
    } else {
        logFuncEnds(FUNCTION_CALLS, "finish cast StrObj from DatatypeObj: %s\n", ( ret->value.string.dyn_str ) );
    }

    return ret;
}


static BoolObj* eqDatatype(object_p op1, DatatypeObj* op2){
    logFuncStart(FUNCTION_CALLS, "start equals DatatypeObj to DatatypeObj\n");

    BoolObj* ret = constructBoolObj(  ((DatatypeObj*)op1)->value == op2->value  );

    logFuncEnds(FUNCTION_CALLS, "finish equals DatatypeObj to DatatypeObj: %u\n", ret->value);

    return ret;
}

static BoolObj* noteqDatatype(object_p op1, DatatypeObj* op2){
    logFuncStart(FUNCTION_CALLS, "start equals DatatypeObj to DatatypeObj\n");

    BoolObj* ret = constructBoolObj(  ((DatatypeObj*)op1)->value != op2->value  );

    logFuncEnds(FUNCTION_CALLS, "finish equals DatatypeObj to DatatypeObj: %u\n", ret->value);

    return ret;
}