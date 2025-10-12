#ifndef NNPSTR_H
#define NNPSTR_H


#include <stdint.h>


#define NNPSTR_UNIONMODE_BUFFER 0
#define NNPSTR_UNIONMODE_POINTER 1


#define STRING_U_BUFFER_NUM_CHARS 7
#define STRING_U_BUFFER_SIZE 8



typedef void* object_p;
typedef struct _NnpStr NnpStr;
typedef struct _Storage Storage;
typedef struct _ObjArray ObjArray;
typedef unsigned int uint;




union string_u {
    char buffer[STRING_U_BUFFER_SIZE];
    char* dyn_str;
};

struct _NnpStr {
    union string_u string;
    uint8_t union_mode;
};

uint8_t NnpStrEq(const NnpStr* str1, const NnpStr* str2);

NnpStr makeNnpStr(const char* str);

void setNnpStr(const char* str, NnpStr* p_nnpStr);

void freeNnpStr(NnpStr* p_nnpStr);

void catNnpStrs(NnpStr* src1, const NnpStr* src2);

#endif