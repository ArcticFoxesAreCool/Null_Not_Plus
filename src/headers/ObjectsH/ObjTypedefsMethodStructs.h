#ifndef OBJTYPEDEFSMETHODSTRUCTS_H
#define OBJTYPEDEFSMETHODSTRUCTS_H






#include "../DynamicValuesH/NnpStr.h"
#include "../DynamicValuesH/ObjArray.h"
#include "../DynamicValuesH/Storage.h"

#include "DatatypeEnum.h"


typedef struct _NumObj NumObj;
typedef struct _StrObj StrObj;
typedef struct _BoolObj BoolObj;
typedef struct _DatatypeObj DatatypeObj;
typedef struct _ListObj ListObj;

typedef void* object_p;
typedef unsigned int uint;

typedef struct _OperatorsStruct OperatorsStruct;
typedef struct _BuiltMethodsStruct BuiltMethodsStruct;










struct _OperatorsStruct{

    object_p (*_add_num)(object_p, NumObj*);//0
    object_p (*_sub_num)(object_p, NumObj*);
    object_p (*_mult_num)(object_p, NumObj*);
    object_p (*_div_num)(object_p, NumObj*);//3


    object_p (*_add_str)(object_p, StrObj*);
    object_p (*_sub_str)(object_p, StrObj*);
    object_p (*_mult_str)(object_p, StrObj*);//6
    object_p (*_div_str)(object_p, StrObj*);//7


    object_p (*_add_bool)(object_p, BoolObj*);
    object_p (*_sub_bool)(object_p, BoolObj*);
    object_p (*_mult_bool)(object_p, BoolObj*);
    object_p (*_div_bool)(object_p, BoolObj*);//11


    object_p (*_add_datatype)(object_p, DatatypeObj*);
    object_p (*_sub_datatype)(object_p, DatatypeObj*);
    object_p (*_mult_datatype)(object_p, DatatypeObj*);
    object_p (*_div_datatype)(object_p, DatatypeObj*);//15


    object_p (*_add_list)(object_p, ListObj*);
    object_p (*_sub_list)(object_p, ListObj*);
    object_p (*_mult_list)(object_p, ListObj*);
    object_p (*_div_list)(object_p, ListObj*);//19






    BoolObj* (*_eq_num)(object_p, NumObj*);//20
    BoolObj* (*_noteq_num)(object_p, NumObj*);
    BoolObj* (*_greaterthan_num)(object_p, NumObj*);
    BoolObj* (*_lessthan_num)(object_p, NumObj*);//23

    
    BoolObj* (*_eq_str)(object_p, StrObj*);
    BoolObj* (*_noteq_str)(object_p, StrObj*);
    BoolObj* (*_greaterthan_str)(object_p, StrObj*);
    BoolObj* (*_lessthan_str)(object_p, StrObj*);


    BoolObj* (*_eq_datatype)(object_p, DatatypeObj*);
    BoolObj* (*_noteq_datatype)(object_p, DatatypeObj*);
    BoolObj* (*_greaterthan_datatype)(object_p, DatatypeObj*);
    BoolObj* (*_lessthan_datatype)(object_p, DatatypeObj*);


    BoolObj* (*_eq_bool)(object_p, BoolObj*);
    BoolObj* (*_noteq_bool)(object_p, BoolObj*);
    BoolObj* (*_greaterthan_bool)(object_p, BoolObj*);
    BoolObj* (*_lessthan_bool)(object_p, BoolObj*);


    BoolObj* (*_eq_list)(object_p, ListObj*);
    BoolObj* (*_noteq_list)(object_p, ListObj*);
    BoolObj* (*_greaterthan_list)(object_p, ListObj*);
    BoolObj* (*_lessthan_list)(object_p, ListObj*);





    NumObj* (*_cast_num)(object_p);//40
    StrObj* (*_cast_str)(object_p);
    BoolObj* (*_cast_bool)(object_p);
    ListObj* (*_cast_list)(object_p);
    DatatypeObj* (*_cast_datatype)(object_p);//44

};


struct _BuiltMethodsStruct {
    //all
    // num
    void* (*truncate)(NumObj*);

    //string
    NumObj* (*str_len)(StrObj*);
    StrObj* (*substring)(StrObj*, NumObj*, NumObj*);
    
    
    //list
    void* (*clear)(ListObj*);
    void* (*pop)(ListObj*, NumObj*);
    object_p (*at_loc)(ListObj*, NumObj*);
    NumObj* (*find)(ListObj*, object_p);
    void* (*insert)(ListObj*, object_p, NumObj*);
    void* (*set)(ListObj*, object_p, NumObj*);

};


#define BUILT_METHODS_STRUCT_NULL {NULL,    NULL,NULL,  NULL,NULL,NULL,NULL,NULL,NULL}


#define OPERATORS_STRUCT_NULL {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,      NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,        NULL,NULL,NULL,NULL,NULL}



#endif