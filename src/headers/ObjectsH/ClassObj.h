#ifndef CLASSOBJ_H
#define CLASSOBJ_H

#include "DatatypeEnum.h"
#include "../DynamicValuesH/Storage.h"
#include "UserFuncObj.h"

typedef struct ClassObj {
    Datatype_e type;
    UserFuncObj* constructor;
    // Storage static_methods;
} ClassObj;

ClassObj* constructClassObj(long tell, int num_parameters, int line_number);


#endif