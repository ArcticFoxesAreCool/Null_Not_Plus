#ifndef TOKENINTERPERTATION_H
#define TOKENINTERPERTATION_H

#include "../DynamicValuesH/ObjArray.h"


enum Token_Types_e {VALUE, VARIABLE, KEYWORD, OPERATOR};

#define INITIAIL_TOKEN_TYPER_LEN 8

typedef struct {
    enum Token_Types_e* types;
    int size;
} TokenTyper;

void initTok_types(int initial_size);

void freeTok_types();

void getTok_types();

// void getAllObjs



#endif