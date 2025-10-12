
#ifndef BOOLOBJ_H
#define BOOLOBJ_H




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







struct _BoolObj{
    Datatype_e type;
    OperatorsStruct* operators;
    BuiltMethodsStruct* methods;
    uint8_t value;
};


BoolObj* constructBoolObj(uint8_t value);



// BoolObj_struct_of_methods* methods;
//     NumObj (*_cast_num)(NumObj*);
//     struct StrObj (*_cast_str)(NumObj*);
//     struct BoolObj (*_cast_bool)(NumObj*);
//     struct ListObj (*_cast_list)(NumObj*);
// DatatypeObj (*get_datatype)(NumObj*);




#endif