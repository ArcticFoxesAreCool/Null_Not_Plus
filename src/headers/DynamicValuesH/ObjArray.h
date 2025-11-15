#ifndef OBJARRAY_H
#define OBJARRAY_H


#define DEFAULT_OBJ_ARRAY_CAPACITY 4


typedef void* object_p;
typedef struct _NnpStr NnpStr;
typedef struct _Storage Storage;
typedef struct _ObjArray ObjArray;
typedef unsigned int uint;



struct _ObjArray{
    object_p* objs;// stores copies
    uint capacity;
    uint length;
};


#define OBJ_PRINTING_CHAR_SIZE 64

char* objValtoDynAllocStr(object_p obj);

object_p copyObj(const object_p this_obj);

void freeObj(object_p this_obj);




void appendInObjArray(ObjArray* p_obj_arr, const object_p this_obj);

void popInObjArray(ObjArray* p_obj_arr, uint index);

void freeObjArrayEntries(ObjArray* p_obj_arr);

void insertInObjArray(ObjArray* p_obj_arr, const object_p this_obj, uint index);


#endif