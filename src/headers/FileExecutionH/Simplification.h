#ifndef EXECUTION_H
#define EXECUTION_H


void executeLine();

object_p constructFromValue(int tok_index);

int getListClosingIndex(int open_bracket_index);

void condenseObjsAndOperators(ObjArray* p_empty_objarr);
void fillLists(ObjArray* p_temp_stack, Datatype_e* datatype_arr);

void initBigStorage(int size);
void deepFreeBigStorage();

BuiltMethodsStruct* getObjMethods(object_p obj);

#endif