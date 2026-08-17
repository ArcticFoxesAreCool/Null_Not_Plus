#ifndef USERFUNCOBJ_H
#define USERFUNCOBJ_H

#include "DatatypeEnum.h"

typedef struct UserFuncObj {
    Datatype_e type;
    long declaration_tell;
    int num_args;
    int line_number;
} UserFuncObj;

UserFuncObj* constructUserFuncObj(long tell, int num_parameters, int line_number);

#endif