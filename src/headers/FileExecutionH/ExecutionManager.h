#ifndef EXECUTIONMANAGER_H
#define EXECUTIONMANAGER_H

#include "../DynamicValuesH/Storage.h"

#define STORAGE_INITIAL_CAPACITY 32

#define SKIP_EXECUTION_UNTIL_INACTIVE -1

enum TypeOfLine {
    LINE_BLANK,
    LINE_ARITHMETIC,
    LINE_RETURN,
    LINE_LOOP,
    LINE_CONDITIONAL,
    LINE_FUNC_DECLARATION,
    LINE_CLASS_DECLARATION,
    LINE_NO_SCOPE
};

struct ScopeTracker {
    int no_nest_level;
    int execution_level;
};

void executeCode(const char* nnp_path);

void executeTheLine(ObjArray* p_line_memory, enum TypeOfLine line_type, Storage* p_store);

void freeNonVarsInObjArr(ObjArray* p_obj_arr, const Storage* p_store);

void initStorage(Storage* p_store, int capacity);

void deepFreeStorage(Storage* p_store);

void endOfLineLogging(const Storage* p_store, bool log_empty_lines);

enum TypeOfLine getCurrentLineType(const Storage* p_store);

#endif