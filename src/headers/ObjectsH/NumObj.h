
#ifndef NUMOBJ_H
#define NUMOBJ_H



#define NUM_TO_STR_BUFFER_SIZE 32




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







struct _NumObj{
    Datatype_e type;
    OperatorsStruct* operators;
    BuiltMethodsStruct* methods;
    double value;
};


NumObj* constructNumObj(double value);




// #define NumObj_NumMethods 10
// #define SIZEOF_FUNC_POINTER 8


#endif