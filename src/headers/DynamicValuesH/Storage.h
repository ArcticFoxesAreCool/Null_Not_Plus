#ifndef STORAGE_H
#define STORAGE_H





typedef void* object_p;
typedef struct _NnpStr NnpStr;
typedef struct _Storage Storage;
typedef struct _ObjArray ObjArray;
typedef unsigned int uint;



struct _Storage{
    object_p* objs; // stores copies
    NnpStr* identifiers;

    uint capacity;
    uint length;
};


object_p getFromStorage(const Storage* p_store, const NnpStr* name);
void setInStorage(Storage* p_store, const NnpStr* name, object_p new_val);

void appendStorage(Storage* p_store, const object_p obj, const NnpStr* name);
void removeStorage(Storage* p_store, const NnpStr* name);

void freeStorage(Storage* p_store);


#endif
