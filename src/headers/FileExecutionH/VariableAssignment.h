#ifndef VARIABLEASSIGNMENT_H
#define VARIABLEASSIGNMENT_H

#include "../DynamicValuesH/Storage.h"
#include "../DynamicValuesH/ObjArray.h"
#include "../DynamicValuesH/NnpStr.h"

void assignVar(Storage* p_store, ObjArray* p_line_stack, const NnpStr* identifier);

void storeUserFuncDeclaration(Storage* p_store, long decl_tell, int line_number);

void storeClassDeclaration(Storage* p_store, long decl_tell, int line_number);

#endif