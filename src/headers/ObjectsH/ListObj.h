#ifndef LISTOBJ_H
#define LISTOBJ_H






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







struct _ListObj{
    Datatype_e type;
    OperatorsStruct* operators;
    BuiltMethodsStruct* methods;
    ObjArray values;
  };


ListObj* constructListObj(const ObjArray* value);


#endif