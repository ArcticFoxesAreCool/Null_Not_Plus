#include "../../headers/datatypes.h"



object_p getFromStorage(const Storage* p_store, const NnpStr* name){
    assert(p_store && name && "get from storage pstore or name null");

    for (uint i = 0; i < p_store->length; i++){
        if (NnpStrEq(  name, &(p_store->identifiers[i])  )){
            return p_store->objs[i];
        }
    }
    return NULL;
    // assert(false && "get from storage name not found");
}

void appendStorage(Storage* p_store, const object_p obj, const NnpStr* name){
    assert(p_store && obj && name && "append storage");
    if (p_store->length == p_store->capacity){
        p_store->capacity *= 2;
        p_store->objs = myRealloc(p_store->objs, sizeof(object_p) * p_store->capacity);
        p_store->identifiers = myRealloc(p_store->identifiers, sizeof(NnpStr) * p_store->capacity);
        assert(p_store->objs && p_store->identifiers && "realloc append storage");
    }
    p_store->objs[p_store->length] = copyObj(obj);
    assert(p_store->objs[p_store->length]);
   
    
    if (name->union_mode == NNPSTR_UNIONMODE_BUFFER){
        setNnpStr(name->string.buffer, &(p_store->identifiers[p_store->length]));
    } else {
        setNnpStr(name->string.dyn_str, &(p_store->identifiers[p_store->length]));
    }


    p_store->length += 1;
}


void removeStorage(Storage* p_store, const NnpStr* name){
    assert(p_store && name && "remove storage");
    for (uint i = 0; i < p_store->length; i++){
        if (NnpStrEq(  name, &(p_store->identifiers[i])  )){
            freeNnpStr(  &(p_store->identifiers[i])  );
            freeObj(  p_store->objs[i]  );

            if (i + 1 != p_store->length){
                memmove(&(p_store->identifiers[i]), &(p_store->identifiers[i + 1]), sizeof(NnpStr) * (p_store->length - i - 1));
                memmove(&(p_store->objs[i]), &(p_store->objs[i + 1]), sizeof(object_p) * (p_store->length - i - 1));
            }

            p_store->length -= 1;
            return;
        }
    }
    assert(false && "name not found remove");
}

void freeStorage(Storage* p_store){
    assert(p_store && "free storage");
    for (uint i = 0; i < p_store->length; i++){
        freeObj( p_store->objs[i] );
        freeNnpStr( &(p_store->identifiers[i]) );
    }
    myFree(p_store->objs);
    myFree(p_store->identifiers);

    myFree(p_store);
}


void setInStorage(Storage* p_store, const NnpStr* name, object_p new_val){
    assert(new_val);
    for (uint i = 0; i < p_store->length; i++){
        if (NnpStrEq(  name, &(p_store->identifiers[i])  )){
            freeObj(p_store->objs[i]);
            p_store->objs[i] = copyObj(new_val);
            return;
        }
    }
}