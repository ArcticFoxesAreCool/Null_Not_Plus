
#include "../../headers/datatypes.h"



static object_p addBool(object_p op1, BoolObj* op2);
static object_p addDatatype(object_p op1, DatatypeObj* op2);
static object_p addList(object_p op1, ListObj* op2);
static object_p addNum(object_p op1, NumObj* op2);
static object_p addStr(object_p op1, StrObj* op2);
static DatatypeObj* castDatatype(__unused object_p op);
static StrObj* castStr(object_p op);
static ListObj* castList(object_p op);
static BoolObj* castBool(object_p op);
static NumObj* castNum(object_p op);
static BoolObj* eqList(object_p op1, ListObj* op2);
static BoolObj* noteqList(object_p op1, ListObj* op2);



OperatorsStruct listOperators = {
    ._add_bool = addBool,// appends to the list
    ._add_datatype = addDatatype,
    ._add_list = addList,
    ._add_num = addNum,
    ._add_str = addStr,

    ._cast_datatype = castDatatype,
    ._cast_str = castStr,// ??????
    ._cast_list = castList,// returns a copy
    ._cast_bool = castBool,// contains an element
    ._cast_num = castNum,// returns length

    ._eq_list = eqList,
    ._noteq_list = noteqList
};




static void* big_clear(ListObj* lst);
static void* pop_it(ListObj* lst, NumObj* index);
static object_p at_location(ListObj* lst, NumObj* index);
static NumObj* find_it(ListObj* lst, object_p obj);
static void* insert_obj(ListObj* lst, object_p obj, NumObj* index);
static void* set_lst(ListObj* lst, object_p value, NumObj* index);




BuiltMethodsStruct listMethods = {
    .at_loc = at_location,
    .clear = big_clear,
    .find = find_it,
    .insert = insert_obj,
    .pop = pop_it,
    .set = set_lst
};



ListObj* constructListObj(const ObjArray* value){
    logFuncStart(FUNCTION_CALLS, "start con ListObj\n");
    assert(value && "construct list obj");
    
    ListObj* ret = myMalloc(sizeof(ListObj));
    assert(ret && "malloc a ListObj during construction");
    ret->operators = &listOperators;
    ret->methods = &listMethods;
    ret->type = LIST_OBJ;

    ret->values.capacity = value->capacity;
    ret->values.length = value->length;
    ret->values.objs = myMalloc(sizeof(object_p) * value->capacity);
    assert(ret->values.objs && "new list size construction");

    for(uint i = 0; i < value->length; i++){
        ret->values.objs[i] = copyObj(value->objs[i]);
    }

    logFuncEnds(FUNCTION_CALLS, "finish con ListObj\n");

    return ret;
}








static object_p addBool(object_p op1, BoolObj* op2){
    logFuncStart(FUNCTION_CALLS, "start add BoolObj to ListObj\n");
    
    ListObj* ret = copyObj(op1);
    if (ret->values.length == ret->values.capacity){
        ret->values.capacity *= 2;
        ret->values.objs = myRealloc(ret->values.objs, sizeof(object_p) * ret->values.capacity);
        assert(ret->values.objs && "add BoolObj to ListObj realloc fail");
    }
    ret->values.objs[ ret->values.length ] = copyObj(op2);

    logFuncEnds(FUNCTION_CALLS, "finish add BoolObj to ListObj, value at index %u: %u\n", ret->values.length, ((BoolObj*)(ret->values.objs[ret->values.length]))->value  );
    ret->values.length += 1;
    return ret;
}

static object_p addDatatype(object_p op1, DatatypeObj* op2){
    logFuncStart(FUNCTION_CALLS, "start add DatatypeObj to ListObj\n");
    
    ListObj* ret = copyObj(op1);
    if (ret->values.length == ret->values.capacity){
        ret->values.capacity *= 2;
        ret->values.objs = myRealloc(ret->values.objs, sizeof(object_p) * ret->values.capacity);
        assert(ret->values.objs && "add DatatypeObj to ListObj realloc fail");
    }
    ret->values.objs[ ret->values.length ] = copyObj(op2);

    logFuncEnds(FUNCTION_CALLS, "finish add DatatypeObj to ListObj, value at index %u: %u\n", ret->values.length, ((DatatypeObj*)(ret->values.objs[ret->values.length]))->value  );
    ret->values.length += 1;
    return ret;
}

static object_p addList(object_p op1, ListObj* op2){
    logFuncStart(FUNCTION_CALLS, "start add ListObj to ListObj\n");
    
    ListObj* ret = copyObj(op1);
    if (ret->values.length == ret->values.capacity){
        ret->values.capacity *= 2;
        ret->values.objs = myRealloc(ret->values.objs, sizeof(object_p) * ret->values.capacity);
        assert(ret->values.objs && "add ListObj to ListObj realloc fail");
    }
    ret->values.objs[ ret->values.length ] = copyObj(op2);

    logFuncEnds(FUNCTION_CALLS, "finish add ListObj to ListObj, index %u\n", ret->values.length);
    ret->values.length += 1;
    return ret;
}

static object_p addNum(object_p op1, NumObj* op2){
    logFuncStart(FUNCTION_CALLS, "start add NumObj to ListObj\n");
    
    ListObj* ret = copyObj(op1);
    if (ret->values.length == ret->values.capacity){
        ret->values.capacity *= 2;
        ret->values.objs = myRealloc(ret->values.objs, sizeof(object_p) * ret->values.capacity);
        assert(ret->values.objs && "add NumObj to ListObj realloc fail");
    }
    ret->values.objs[ ret->values.length ] = copyObj(op2);

    logFuncEnds(FUNCTION_CALLS, "finish add NumObj to ListObj, value at index %u: %u\n", ret->values.length, ((NumObj*)(ret->values.objs[ret->values.length]))->value  );

    ret->values.length += 1;
    return ret;
}

static object_p addStr(object_p op1, StrObj* op2){
    logFuncStart(FUNCTION_CALLS, "start add StrObj to ListObj\n");
    
    ListObj* ret = copyObj(op1);
    if (ret->values.length == ret->values.capacity){
        ret->values.capacity *= 2;
        ret->values.objs = myRealloc(ret->values.objs, sizeof(object_p) * ret->values.capacity);
        assert(ret->values.objs && "add StrObj to ListObj realloc fail");
    }
    ret->values.objs[ ret->values.length ] = copyObj(op2);

    if (  ((StrObj*)(ret->values.objs[ret->values.length]))->value.union_mode == NNPSTR_UNIONMODE_BUFFER  ){
        logFuncEnds(FUNCTION_CALLS, "finish add StrObj to ListObj, value at index %u: %u\n", ret->values.length, ((StrObj*)(ret->values.objs[ret->values.length]))->value.string.buffer  );
    } else {
        logFuncEnds(FUNCTION_CALLS, "finish add StrObj to ListObj, value at index %u: %u\n", ret->values.length, ((StrObj*)(ret->values.objs[ret->values.length]))->value.string.dyn_str  );
    }

    ret->values.length += 1;
    return ret;
}
static DatatypeObj* castDatatype(__unused object_p op){
    logMessage(FUNCTION_CALLS, "Get Datatype of ListObj\n");
    return constructDatatypeObj(LIST_OBJ);
}

// static StrObj* castStr(object_p op){}

static ListObj* castList(object_p op){
    logFuncStart(FUNCTION_CALLS, "start cast ListObj from ListObj\n");
    ListObj* ret = copyObj(op);
    logFuncEnds(FUNCTION_CALLS, "finish cast ListObj to ListObj\n");
    return ret;
}

static BoolObj* castBool(object_p op){
    logFuncStart(FUNCTION_CALLS, "start cast BoolObj from ListObj\n");
    BoolObj* ret = constructBoolObj(  ((ListObj*)op)->values.length != 0  );
    logFuncEnds(FUNCTION_CALLS, "finish cast BoolObj from ListObj: %u\n", ret->value);
    return ret;
}

static NumObj* castNum(object_p op){
    logFuncStart(FUNCTION_CALLS, "start cast NumObj from ListObj\n");
    NumObj* ret = constructNumObj(  ((ListObj*)op)->values.length  );
    logFuncEnds(FUNCTION_CALLS, "finish cast NumObj from ListObj: %lf\n", ret->value);
    return ret;
}

static StrObj* castStr(object_p op){
    logFuncStart(FUNCTION_CALLS, "start cast StrObj from ListObj\n");

    // puts("AAAAAA"); fflush(stdout);

    char* st = objValtoDynAllocStr(op);
    assert(st);
    // puts("BBBBBB"); fflush(stdout);
    
    StrObj* ret = constructStrObj_char(st);
    myFree(st);
    
    if (ret->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
        logFuncEnds(FUNCTION_CALLS, "finish cast StrObj from ListObj: %s\n", ret->value.string.buffer);
    } else {
        logFuncEnds(FUNCTION_CALLS, "finish cast StrObj from ListObj: %s\n", ret->value.string.dyn_str);
    }
    return ret;
}



static inline BoolObj* eqListReter(uint8_t res){
    BoolObj* ret = constructBoolObj(res);
    logFuncEnds(FUNCTION_CALLS, "finish ListObj eq ListObj: %u\n", ret->value);
    return ret;
}

static BoolObj* eqList(object_p op1, ListObj* op2){
    logFuncStart(FUNCTION_CALLS, "start ListObj eq ListObj\n");
    assert(op1 && op2);

    if (((ListObj*)op1)->values.length != op2->values.length){
        return eqListReter(false);
    }

    Datatype_e dat;
    for(uint i = 0; i < op2->values.length; i++){
        dat = *((Datatype_e*)(op2->values.objs[i]));

        // if objs[i] have different data types
        if (dat !=   *((Datatype_e*)(((ListObj*)op1)->values.objs[i]))  ){
            return eqListReter(false);
        }


        switch (dat){
        case BOOL_OBJ:
            if (  ((BoolObj*)(op2->values.objs[i]))->value != ((BoolObj*)(((ListObj*)op2)->values.objs[i]))->value ){
                return eqListReter(false);
            }
            break;
        case DATATYPE_OBJ:
            if (  ((DatatypeObj*)(op2->values.objs[i]))->value != ((DatatypeObj*)(((ListObj*)op2)->values.objs[i]))->value ){
                return eqListReter(false);
            }
            break;
        case NUM_OBJ:
            if (  ((NumObj*)(op2->values.objs[i]))->value != ((NumObj*)(((ListObj*)op2)->values.objs[i]))->value ){
                return eqListReter(false);
            }
            break;
        case STR_OBJ:
            {
                BoolObj* temp = ((StrObj*)(op2->values.objs[i]))->operators->_eq_str(op2->values.objs[i], ((StrObj*)(((ListObj*)op1)->values.objs[i]))  );
                if (temp->value == false){
                    logFuncEnds(FUNCTION_CALLS, "finish ListObj eq ListObj: %u\n", temp->value);
                    return temp;
                }
                freeObj(temp);
                break;
            }
        case LIST_OBJ:
            {
                BoolObj* temp = eqList(  ((ListObj*)(((ListObj*)op1)->values.objs[i])), ((ListObj*)(op2->values.objs[i]))  );
                if (temp->value == false){
                    logFuncEnds(FUNCTION_CALLS, "finish ListObj eq ListObj: %u\n", temp->value);
                    return temp;
                }
                break;
            }
        default:
            assert(false && "datatype value not found");
        }
    }

    return eqListReter(true);
}







static inline BoolObj* notEqListReter(uint8_t res){
    BoolObj* ret = constructBoolObj(res);
    logFuncEnds(FUNCTION_CALLS, "finish ListObj not eq ListObj: %u\n", ret->value);
    return ret;
}

static BoolObj* noteqList(object_p op1, ListObj* op2){
logFuncStart(FUNCTION_CALLS, "start ListObj not eq ListObj\n");
    assert(op1 && op2);

    if (((ListObj*)op1)->values.length != op2->values.length){
        return notEqListReter(true);
    }

    Datatype_e dat;
    for(uint i = 0; i < op2->values.length; i++){
        dat = *((Datatype_e*)(op2->values.objs[i]));

        // if objs[i] have different data types
        if (dat !=   *((Datatype_e*)(((ListObj*)op1)->values.objs[i]))  ){
            return notEqListReter(true);
        }


        switch (dat){
        case BOOL_OBJ:
            if (  ((BoolObj*)(op2->values.objs[i]))->value != ((BoolObj*)(((ListObj*)op2)->values.objs[i]))->value ){
                return notEqListReter(true);
            }
            break;
        case DATATYPE_OBJ:
            if (  ((DatatypeObj*)(op2->values.objs[i]))->value != ((DatatypeObj*)(((ListObj*)op2)->values.objs[i]))->value ){
                return notEqListReter(true);
            }
            break;
        case NUM_OBJ:
            if (  ((NumObj*)(op2->values.objs[i]))->value != ((NumObj*)(((ListObj*)op2)->values.objs[i]))->value ){
                return notEqListReter(true);
            }
            break;
        case STR_OBJ:
            {
                BoolObj* temp = ((StrObj*)(op2->values.objs[i]))->operators->_noteq_str(op2->values.objs[i], ((StrObj*)(((ListObj*)op1)->values.objs[i]))  );
                if (temp->value == true){
                    logFuncEnds(FUNCTION_CALLS, "finish ListObj not eq ListObj: %u\n", temp->value);
                    return temp;
                }
                freeObj(temp);
                break;
            }
        case LIST_OBJ:
            {
                BoolObj* temp = noteqList(  ((ListObj*)(((ListObj*)op1)->values.objs[i])), ((ListObj*)(op2->values.objs[i]))  );
                if (temp->value == true){
                    logFuncEnds(FUNCTION_CALLS, "finish ListObj not eq ListObj: %u\n", temp->value);
                    return temp;
                }
                break;
            }
        default:
            assert(false && "datatype value not found");
        }
    }

    return notEqListReter(false);
}























static void* big_clear(ListObj* lst){
    logFuncStart(FUNCTION_CALLS, "start clear method ListObj\n");
    for(uint i = 0; i < lst->values.length; i++){
        freeObj(lst->values.objs[i]);
    }
    lst->values.length = 0;

    lst->values.capacity = DEFAULT_OBJ_ARRAY_CAPACITY;
    myFree(lst->values.objs);
    lst->values.objs = myMalloc(sizeof(object_p) * DEFAULT_OBJ_ARRAY_CAPACITY);
    assert(lst->values.objs);

    logFuncEnds(FUNCTION_CALLS, "finish clear method ListObj\n");
    return NULL;
}



static void* pop_it(ListObj* lst, NumObj* index){
    logFuncStart(FUNCTION_CALLS, "start pop method ListObj\n");
    assert(lst && index);
    assert(index->value >= 0.0);

    uint the_index = (uint)trunc(index->value);
    assert(the_index < lst->values.length);

    freeObj(lst->values.objs[the_index]);
    lst->values.length--;
    if (the_index != lst->values.length){
        memmove(&(lst->values.objs[the_index]), &(lst->values.objs[the_index+1]), (lst->values.length - the_index) * sizeof(object_p));
    }
    logFuncEnds(FUNCTION_CALLS, "finish pop method ListObj\n");
    return NULL;
}



static object_p at_location(ListObj* lst, NumObj* index){
    logFuncStart(FUNCTION_CALLS, "start at_loc method ListObj\n");
    assert(lst && index);
    assert(index->value >= 0.0);

    uint the_index = (uint)trunc(index->value);
    assert(the_index < lst->values.length);
    logFuncEnds(FUNCTION_CALLS, "finish at_loc method ListObj\n");
    return copyObj(lst->values.objs[the_index]);
}



static inline NumObj* find_it_reter(uint index){
    NumObj* ret = constructNumObj((double)index);
    logFuncEnds(FUNCTION_CALLS, "finish find method ListObj: %lf\n", ret->value);
    return ret;
}

static NumObj* find_it(ListObj* lst, object_p obj){
    logFuncStart(FUNCTION_CALLS, "start find method ListObj\n");
    assert(lst && obj);
    Datatype_e obj_type = *((Datatype_e*)obj);
    Datatype_e dat;
    for(uint i = 0; i < lst->values.length; i++){
        dat = *((Datatype_e*)(lst->values.objs[i]));
        if (obj_type != dat){continue;}

        switch (dat){
        case BOOL_OBJ:
            if (((BoolObj*)obj)->value == ((BoolObj*)(lst->values.objs[i]))->value){
                return find_it_reter(i);
            }
            break;
        case DATATYPE_OBJ:
            if (((DatatypeObj*)obj)->value == ((DatatypeObj*)(lst->values.objs[i]))->value){
                return find_it_reter(i);
            }
            break;
        case LIST_OBJ:
            {
                BoolObj* temp = eqList(lst->values.objs[i], (ListObj*)obj);
                if (temp->value != false){
                    freeObj(temp);
                    return find_it_reter(i);
                }
                freeObj(temp);
                break;
            }
        case NUM_OBJ:
            if (  fabs(  ((NumObj*)obj)->value - ((NumObj*)(lst->values.objs[i]))->value ) <= 2 * __DBL_EPSILON__ ){
                return find_it_reter(i);
            }
            break;
        case STR_OBJ:
            {
                BoolObj* temp = ((StrObj*)(lst->values.objs[i]))->operators->_eq_str(lst->values.objs[i], (StrObj*)obj);
                if (temp->value != false){
                    freeObj(temp);
                    return find_it_reter(i);
                }
                freeObj(temp);
            }
            break;
        default:
            assert(false && "could not find object datatype");
        }
    }
    NumObj* ret = constructNumObj(NAN);
    logFuncEnds(FUNCTION_CALLS, "finish find method ListObj: %lf\n", ret->value);
    return ret;
}

static void* insert_obj(ListObj* lst, object_p obj, NumObj* index){
    logFuncStart(FUNCTION_CALLS, "start insert method ListObj\n");

    assert(lst && obj && index);
    assert(index->value >= 0);
    uint the_index = (uint)index->value;
    assert(lst->values.length >= the_index);

    if (lst->values.length == lst->values.capacity){
        lst->values.capacity *= 2;
        lst->values.objs = myRealloc(lst->values.objs, sizeof(object_p) * lst->values.capacity);
        assert(lst->values.objs);
    }
    if (the_index != lst->values.length){
        memmove(&(lst->values.objs[the_index + 1]), &(lst->values.objs[the_index]), (lst->values.length - the_index) * sizeof(object_p));
    }
    lst->values.length++;
    lst->values.objs[the_index] = copyObj(obj);
    logFuncEnds(FUNCTION_CALLS, "finish insert method ListObj\n");
    return NULL;
}

static void* set_lst(ListObj* lst, object_p value, NumObj* index){
    logFuncStart(FUNCTION_CALLS, "start set method ListObj\n");
    assert(lst && value && index && index->value >= 0.0);
    uint ind = (uint)(index->value);
    assert(ind < lst->values.length);
    
    if (lst->values.objs[ind] != NULL){
        freeObj(lst->values.objs[ind]);
    }
    lst->values.objs[ind] = copyObj(value);

    logFuncEnds(FUNCTION_CALLS, "finish set method ListObj\n");
    return NULL;
}