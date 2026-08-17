#ifndef INSTANCEOBJ_H
#define INSTANCEOBJ_H

#include "DatatypeEnum.h"
#include "ClassObj.h"
#include "UserFuncObj.h"
#include "../DynamicValuesH/Storage.h"
#include "../FileParsingH/KeywordsAndOperators.h"


typedef struct InstanceObj {
    Datatype_e type;
    Storage vars_methods;
    
    long _constructor_tell; // used for instanceof check
    // UserFuncObj* operators[NUM_OPERATORS];
} InstanceObj;


InstanceObj* constructInstanceObj(ClassObj* class);

#endif