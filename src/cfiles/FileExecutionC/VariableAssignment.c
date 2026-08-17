#include "../../headers/datatypes.h"

void assignVar(Storage* p_store, ObjArray* p_line_stack, const NnpStr* p_identifier){
    assert(p_store && p_line_stack && p_identifier);
    assert(p_store->objs && p_store->identifiers && p_store->capacity >= 1);
    assert(p_line_stack->objs && p_line_stack->capacity >= 1 && p_line_stack->length >= 1);
    
    object_p in_store = getFromStorage(p_store, p_identifier);
    if (in_store){
        setInStorage(p_store, p_identifier, p_line_stack->objs[p_line_stack->length - 1]);
    } else {
        appendStorage(p_store, p_line_stack->objs[p_line_stack->length - 1], p_identifier);
    }
    popInObjArray(p_line_stack, p_line_stack->length - 1);
}


void storeUserFuncDeclaration(Storage* p_store, long decl_tell, int line_number){
    // f <- args

    extern Reader nian;
    extern TokenTyper tok_types;

    assert(p_store);
    assert(decl_tell >= 0);
    assert(nian.charv && nian.sz >= 1 && nian.tok_ind_capacity >= 2 && nian.tok_ind_len >= 2);
    assert(tok_types.size >= 2 && tok_types.types);

    assert(tok_types.types[0] == VARIABLE && tok_types.types[1] == OPERATOR);
    for (int i = 2; i < nian.tok_ind_len; i++) {assert(tok_types.types[i] == VARIABLE);}

    UserFuncObj* user_func = constructUserFuncObj(decl_tell, nian.tok_ind_len - 2, line_number);
    NnpStr func_name = makeNnpStr(nian.charv);
    
    appendStorage(p_store, user_func, &func_name);
    freeNnpStr(&func_name);
    freeObj(user_func);

}


void storeClassDeclaration(Storage* p_store, long decl_tell, int line_number){
    extern Reader nian;
    extern TokenTyper tok_types;

    assert(p_store);
    assert(decl_tell >= 0);
    assert(nian.charv && nian.sz >= 1 && nian.tok_ind_capacity >= 2 && nian.tok_ind_len >= 2);
    assert(tok_types.size >= 2 && tok_types.types);

    assert(tok_types.types[0] == VARIABLE && tok_types.types[1] == OPERATOR);
    for (int i = 2; i < nian.tok_ind_len; i++) {assert(tok_types.types[i] == VARIABLE);}

    ClassObj* user_class = constructClassObj(decl_tell, nian.tok_ind_len - 2, line_number);
    NnpStr class_name = makeNnpStr(nian.charv);

    appendStorage(p_store, user_class, &class_name);
    freeNnpStr(&class_name);
    freeObj(user_class);
}