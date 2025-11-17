#ifndef EXECUTION_H
#define EXECUTION_H



object_p constructFromValue(int tok_index);

int getListClosingIndex(int open_bracket_index);

void condenseObjsAndOperators(ObjArray* obj_line_stack);

void subCondenseObjsOperators(ObjArray* p_temp_stack, Datatype_e* datatype_arr, int start_index, int stop_index);

void fillLists(ObjArray* p_temp_stack, Datatype_e* datatype_arr);


BuiltMethodsStruct* getObjMethods(object_p obj);

#endif