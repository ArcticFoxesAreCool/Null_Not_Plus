#ifndef EXECUTION_H
#define EXECUTION_H


void executeLine();

object_p constructFromValue(int tok_index);

void condenseObjsAndOperators(ObjArray* p_empty_objarr);

void initBigStorage(int size);
void deepFreeBigStorage();

#endif