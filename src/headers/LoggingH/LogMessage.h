
#ifndef LOGMESSAGE_H
#define LOGMESSAGE_H


#include <stdarg.h>

#include "./../DynamicValuesH/Storage.h"

enum LoggingFileType {MEMORY_STATE, FUNCTION_CALLS, FILE_PARSING, OUT};


void initLogging();
void closeLogging();

void setMemoryStateFile(const char* path);
void setFunctionCallsFile(const char* path);
void setFileParsingFile(const char* path);


void logMessage(enum LoggingFileType whichFile, const char* fmt, ...);


void logFuncEnds(enum LoggingFileType whichFile, const char* fmt, ...);
void logFuncStart(enum LoggingFileType whichFile, const char* fmt, ...);

void flushLogs();

void logVariables(enum LoggingFileType whichFile, Storage* p_store, bool printPrebuiltFuncs);

#endif

