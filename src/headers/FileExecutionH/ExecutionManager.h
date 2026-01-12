#ifndef EXECUTIONMANAGER_H
#define EXECUTIONMANAGER_H

#include "../DynamicValuesH/Storage.h"

#define SKIP_EXECUTION_UNTIL_INACTIVE -1

enum TypeOfLine {
    LINE_BLANK,
    LINE_ARITHMETIC,
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

void initBigStorage(int size);

void deepFreeBigStorage();

void endOfLineLogging(bool log_empty_lines);

enum TypeOfLine getCurrentLineType(const Storage* p_store);

#endif