#ifndef FUNCOBJ_H
#define FUNCOBJ_H




#include <stdio.h>

// #include "../DynamicValuesH/NnpStr.h"
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


#define NUM_PREBUILTS_IDENTIFIERS 10


enum PrebuiltFuncs_e {
    USER_FUNC,
    TRUNCATE_M, STR_LEN_M, SUBSTRING_M, POP_M, FIND_M, INSERT_M, SET_M,
    SPEAK_F, LISTEN_F, ASCII_TO_CH_F
};



typedef struct {
    Datatype_e type;
    enum PrebuiltFuncs_e func_type;
    int num_args;
    fpos_t f_loc;
} FuncObj;


void loadPrebuiltsIntoStorage(Storage* p_store);


#endif