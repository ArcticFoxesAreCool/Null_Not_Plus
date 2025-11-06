#include "../../headers/datatypes.h"

void assignVar(Storage* p_store, ObjArray* p_line_stack, const NnpStr* p_identifier){
    assert(p_store && p_line_stack && p_identifier);
    assert(p_store->objs && p_store->identifiers && p_store->capacity >= 1);
    assert(p_line_stack->objs && p_line_stack->capacity >= 1 && p_line_stack->length == 1);
    
    object_p in_store = getFromStorage(p_store, p_identifier);
    if (in_store){
        setInStorage(p_store, p_identifier, p_line_stack->objs[p_line_stack->length - 1]);
    } else {
        appendStorage(p_store, p_line_stack->objs[p_line_stack->length - 1], p_identifier);
    }
    popInObjArray(p_line_stack, p_line_stack->length - 1);
}





