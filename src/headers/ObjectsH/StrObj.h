
#ifndef STROBJ_H
#define STROBJ_H







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






struct _StrObj {
    Datatype_e type;
    OperatorsStruct* operators;
    BuiltMethodsStruct* methods;
    NnpStr value;
};



StrObj* constructStrObj_char(const char* nullEndedStr);


#endif