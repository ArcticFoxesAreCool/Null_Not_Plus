
#include "../../headers/datatypes.h"



static object_p addNum(object_p op1, NumObj* op2);
static object_p subNum(object_p op1, NumObj* op2);
static object_p multNum(object_p op1, NumObj* op2);
static object_p divNum(object_p op1, NumObj* op2);
static object_p multStr(object_p op1, StrObj* op2);

static BoolObj* eqNum(object_p op1, NumObj* op2);
static BoolObj* noteqNum(object_p op1, NumObj* op2);
static BoolObj* greaterNum(object_p op1, NumObj* op2);
static BoolObj* smallerNum(object_p op1, NumObj* op2);

static NumObj* castNum(object_p op);
static BoolObj* castBool(object_p op);
static DatatypeObj* castDatatype(__unused object_p op);
static ListObj* castList(object_p op);
static StrObj* castStr(object_p op);



OperatorsStruct numOperators = {
    ._add_num = addNum,
    ._sub_num = subNum,
    ._mult_num = multNum,
    ._div_num = divNum,
    ._mult_str = multStr,// returns the string repeated number times

    ._eq_num = eqNum,
    ._noteq_num = noteqNum,
    ._greaterthan_num = greaterNum,
    ._lessthan_num = smallerNum,

    ._cast_num = castNum,
    ._cast_bool = castBool,// is not 0
    ._cast_datatype = castDatatype,
    ._cast_list = castList,
    ._cast_str = castStr
};


static void* trun(NumObj* op);



BuiltMethodsStruct numMethods = {
    .truncate = trun
};






NumObj* constructNumObj(double value){
    logFuncStart(FUNCTION_CALLS, "start con NumObj: %lf\n", value);

    NumObj* ret = myMalloc(sizeof(NumObj));
    assert(ret && "Malloc a new num fail\n");

    ret->operators = &numOperators;
    ret->methods = &numMethods;
    ret->type = NUM_OBJ;
    ret->value = value;
    
    logFuncEnds(FUNCTION_CALLS, "finish con NumObj: %lf\n", ret->value);
    return ret;
}







// object_p (*_add_num)(object_p, NumObj*);
// object_p (*_sub_num)(object_p, NumObj*);
// object_p (*_mult_num)(object_p, NumObj*);
// object_p (*_div_num)(object_p, NumObj*);



static object_p addNum(object_p op1, NumObj* op2){
    logFuncStart(FUNCTION_CALLS, "start add NumObj to NumObj\n");
    
    double op1_value = ((NumObj*)op1)->value;
    NumObj* ret = constructNumObj(op1_value + op2->value);

    logFuncEnds(FUNCTION_CALLS, "finish add NumObj to NumObj: %lf\n", ret->value);
    return (object_p)ret;
}


static object_p subNum(object_p op1, NumObj* op2){
    logFuncStart(FUNCTION_CALLS, "start sub NumObj to NumObj\n");

    double op1_value = ((NumObj*)op1)->value;
    NumObj* ret = constructNumObj(op1_value - op2->value);

    logFuncEnds(FUNCTION_CALLS, "finish sub NumObj to NumObj: %lf\n", ret->value);

    return (object_p)ret;
}

static object_p multNum(object_p op1, NumObj* op2){
    logFuncStart(FUNCTION_CALLS, "start mult NumObj to NumObj\n");

    double op1_value = ((NumObj*)op1)->value;
    NumObj* ret = constructNumObj(op1_value * op2->value);

    logFuncEnds(FUNCTION_CALLS, "finish mult NumObj to NumObj: %lf\n", ret->value);

    return (object_p)ret;
}


static object_p divNum(object_p op1, NumObj* op2){
    logFuncStart(FUNCTION_CALLS, "start div NumObj to NumObj\n");

    double op1_value = ((NumObj*)op1)->value;
    NumObj* ret = constructNumObj(op1_value / op2->value);

    logFuncEnds(FUNCTION_CALLS, "finish div NumObj to NumObj: %lf\n", ret->value);

    return (object_p)ret;
}


static object_p multStr(object_p op1, StrObj* op2){
    if (op2->value.union_mode == NNPSTR_UNIONMODE_BUFFER)
        logFuncStart(FUNCTION_CALLS, "start mult NumObj to StrObj (%s)\n", op2->value.string.buffer);
    else
        logFuncStart(FUNCTION_CALLS, "start mult NumObj to StrObj (%s)\n", op2->value.string.dyn_str);

    uint new_str_len;
    assert(op1 && op2 && "num mult str");
    
    uint op1Value =  (uint)( ((NumObj*)op1)->value  );
    assert(  op1Value >= 0  );
    if (op1Value == 0){
        logFuncEnds(FUNCTION_CALLS, "finish mult NumObj to StrObj ()\n");
        return constructStrObj_char("");
    }

    if (op2->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        new_str_len = op1Value * strnlen(op2->value.string.buffer, STRING_U_BUFFER_SIZE);
    } else {
        new_str_len = op1Value * strlen(op2->value.string.dyn_str);
    }



    char string[new_str_len + 1];
    string[0] = '\0';

    if (op2->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        for (uint i = 0; i < op1Value; i++)
            strncat(string, op2->value.string.buffer, STRING_U_BUFFER_SIZE);
    } else {
        for (uint i = 0; i < op1Value; i++)
            strcat(string, op2->value.string.dyn_str);
    }

    StrObj* ret = constructStrObj_char(string);

    if (ret->value.union_mode == NNPSTR_UNIONMODE_BUFFER)
        logFuncEnds(FUNCTION_CALLS, "finish mult NumObj to StrObj (%s)\n", ret->value.string.buffer);
    else
        logFuncEnds(FUNCTION_CALLS, "finish mult NumObj to StrObj (%s)\n", ret->value.string.dyn_str);

    return (object_p)ret;
}

// BoolObj* (*_eq_num)(object_p, NumObj*);
// BoolObj* (*_noteq_num)(object_p, NumObj*);
// BoolObj* (*_greaterthan_num)(object_p, NumObj*);
// BoolObj* (*_lessthan_num)(object_p, NumObj*);

static BoolObj* eqNum(object_p op1, NumObj* op2){
    logFuncStart(FUNCTION_CALLS, "start equals NumObj to NumObj\n");

    // BoolObj* ret = constructBoolObj(f);

    double op1_value = ((NumObj*)op1)->value;
    uint8_t the_value = fabs(op1_value - op2->value) < 2 * __DBL_EPSILON__;
    BoolObj* ret = constructBoolObj(the_value);

    logFuncEnds(FUNCTION_CALLS, "finish equals NumObj to NumObj: %u\n", ret->value);

    return ret;
}

static BoolObj* noteqNum(object_p op1, NumObj* op2){
    logFuncStart(FUNCTION_CALLS, "start not equals NumObj to NumObj\n");
   
    double op1_value = ((NumObj*)op1)->value;
    uint8_t the_value = fabs(op1_value - op2->value) > 2 * __DBL_EPSILON__;

    BoolObj* ret = constructBoolObj(the_value);

    logFuncEnds(FUNCTION_CALLS, "finish not equals NumObj to NumObj: %u\n", ret->value);

    return ret;
}


static BoolObj* greaterNum(object_p op1, NumObj* op2){
    logFuncStart(FUNCTION_CALLS, "start greater than NumObj to NumObj\n");
   
    double op1_value = ((NumObj*)op1)->value;
    uint8_t are_equal = fabs(op1_value - op2->value) < 2 * __DBL_EPSILON__;
    if (are_equal){
        BoolObj* ret = constructBoolObj(false);
        logFuncEnds(FUNCTION_CALLS, "finish greater than NumObj to NumObj: %u\n", ret->value);
        return ret;
    }
    BoolObj* ret = constructBoolObj(op1_value > op2->value);
    logFuncEnds(FUNCTION_CALLS, "finish greater than NumObj to NumObj: %u\n", ret->value);
    return ret;
}


static BoolObj* smallerNum(object_p op1, NumObj* op2){
    logFuncStart(FUNCTION_CALLS, "start smaller than NumObj to NumObj\n");

    double op1_value = ((NumObj*)op1)->value;
    uint8_t are_equal = fabs(op1_value - op2->value) < 2 * __DBL_EPSILON__;
    if (are_equal){
        BoolObj* ret = constructBoolObj(false);
        logFuncEnds(FUNCTION_CALLS, "finish smaller than NumObj to NumObj: %u\n", ret->value);
        return ret;
    }

    BoolObj* ret = constructBoolObj(op1_value < op2->value);
    logFuncEnds(FUNCTION_CALLS, "finish smaller than NumObj to NumObj: %u\n", ret->value);
    return ret;
}







static NumObj* castNum(object_p op){
    logFuncStart(FUNCTION_CALLS, "start cast NumObj from NumObj\n");
    NumObj* ret = constructNumObj(  ((NumObj*)op)->value  );
    logFuncEnds(FUNCTION_CALLS, "finish cast NumObj to NumObj: %lf\n", ret->value);
    return ret;
}


static BoolObj* castBool(object_p op){
    uint8_t is_not_zero = fabs( ((NumObj*)op)->value  ) > 2 * __DBL_EPSILON__;

    logFuncStart(FUNCTION_CALLS, "start cast BoolObj from NumObj\n");
    BoolObj* ret = constructBoolObj(is_not_zero);
    logFuncEnds(FUNCTION_CALLS, "finish cast NumObj to NumObj: %u\n", ret->value);
    return ret;
}

static DatatypeObj* castDatatype(__unused object_p op){
    logMessage(FUNCTION_CALLS, "Get Datatype of NumObj\n");
    return constructDatatypeObj(NUM_OBJ);
}

static ListObj* castList(object_p op){
    logFuncStart(FUNCTION_CALLS, "start cast ListObj from NumObj\n");

    ObjArray temp = {
        .capacity = DEFAULT_OBJ_ARRAY_CAPACITY,
        .length = 1,
        .objs = myMalloc(sizeof(object_p) * DEFAULT_OBJ_ARRAY_CAPACITY)
    };
    temp.objs[0] = copyObj(op);
    
    ListObj* ret = constructListObj(&temp);
    myFree(temp.objs[0]);
    myFree(temp.objs);

    logFuncEnds(FUNCTION_CALLS, "finish cast ListObj from NumObj, value at index 0: %lf\n", ( (NumObj*)(ret->values.objs[0]) )->value );
    return ret;
}


static StrObj* castStr(object_p op){
    logFuncStart(FUNCTION_CALLS, "start cast StrObj from NumObj\n");

    char temp[NUM_TO_STR_BUFFER_SIZE];
    snprintf(temp, sizeof(temp), "%F", ((NumObj*)op)->value);
    StrObj* ret = constructStrObj_char(temp);

    if (ret->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        logFuncEnds(FUNCTION_CALLS, "finish cast NumObj to StrObj buffer: %s\n", ret->value.string.buffer);
    } else {
        logFuncEnds(FUNCTION_CALLS, "finish cast NumObj to StrObj dyn: %s\n", ret->value.string.dyn_str);
    }
    
    return ret;
}


















// void initNumOperators(){
//     numOperators._add_num = addNum;
//     numOperators._sub_num = subNum;
//     numOperators._mult_num = multNum;
//     numOperators._div_num = divNum;

//     numOperators._eq_num = eqNum;
//     numOperators._noteq_num = noteqNum;
//     numOperators._greaterthan_num = greaterNum;
//     numOperators._lessthan_num = smallerNum;

//     numOperators._cast_num = castNum;
//     numOperators._cast_str = NULL;
//     numOperators._cast_bool = castBool;
//     numOperators._cast_list = NULL;
// }



static void* trun(NumObj* op){
    logFuncStart(FUNCTION_CALLS, "start truncate method NumObj\n");
    op->value = trunc(op->value);
    logFuncEnds(FUNCTION_CALLS, "finish truncate method NumObj: %.4lf\n", op->value);
    return NULL;
}









//     NumObj (*truncate)(NumObj*);
    
//     struct DatatypeObj (*get_datatype)(NumObj*);

//     NumObj (*_add_num)(NumObj*, NumObj*);
//     NumObj (*_sub_num)(NumObj*, NumObj*);
//     NumObj (*_mult_num)(NumObj*, NumObj*);
//     NumObj (*_div_num)(NumObj*, NumObj*);

//     NumObj (*_cast_num)(NumObj*);
//     struct StrObj (*_cast_str)(NumObj*);
//     struct BoolObj (*_cast_bool)(NumObj*);
//     struct ListObj (*_cast_list)(NumObj*);

