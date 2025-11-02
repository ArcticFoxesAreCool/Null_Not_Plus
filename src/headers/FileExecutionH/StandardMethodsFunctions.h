#ifndef STANDARDMETHODSFUNCTIONS_H
#define STANDARDMETHODSFUNCTIONS_H

// typedef struct {
//     NnpStr identifier;
//     int num_args;
//     Datatype_e 
// } funcData;

int getFunctionFinalParameterIndex(int num_params, int larrow_index, int stop_index, const Storage* p_store);

void resolveFunction(ObjArray* p_obj_arr, int num_args);
    // resolveFunction(p_obj_arr, func_ref->num_args);




#endif