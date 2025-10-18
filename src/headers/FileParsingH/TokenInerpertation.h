#ifndef TOKENINTERPERTATION_H
#define TOKENINTERPERTATION_H

#include "../DynamicValuesH/ObjArray.h"
#include "../ObjectsH/DatatypeEnum.h"


enum Token_Types_e {VARIABLE, VALUE, KEYWORD, OPERATOR};

#define INITIAIL_TOKEN_TYPER_SIZE 8

typedef struct {
    enum Token_Types_e* types;
    int size;
} TokenTyper;

void initTok_types(int initial_size);

void freeTok_types();

void getTok_types();

Datatype_e findValueType(const char* token);


// void getAllObjs



#endif