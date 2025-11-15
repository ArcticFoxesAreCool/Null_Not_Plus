#include "../../headers/datatypes.h"



static FILE* funcCall = NULL;
static FILE* memState = NULL;
static FILE* fileParse = NULL;

static int functionDepth = 0;

static bool memStateActive = false;
static bool funcCallActive = false;
static bool fileParseActive = false;


void initLogging(){
    if (funcCall){
        fclose(funcCall);
    }
    if (memState){
        fclose(memState);
    }
    if (fileParse){
        fclose(fileParse);
    }

    
    funcCall = fopen("logs/functioncalls.txt", "w");
    assert(funcCall && "open functioncalls.txt");
    funcCallActive = true;
    functionDepth = 0;

    memState = fopen("logs/memorystates.txt", "w");
    assert(memState && "open memorystatess.txt");
    memStateActive = true;

    fileParse = fopen("logs/fileparsing.txt", "w");
    assert(fileParse && "open fileparsing.txt");
    fileParseActive = true;
}


void closeLogging(){
    if (funcCall){
        fclose(funcCall);
    }
    if (memState){
        fclose(memState);
    }
    if (fileParse){
        fclose(fileParse);
    }

    funcCallActive = false;
    memStateActive = false;
    fileParseActive = false;
    functionDepth = 0;
}


void setMemoryStateFile(const char* path){
    if (memState){
        fclose(memState);
    }

    memState = fopen(path, "w");
    assert(memState && "setting memorystatess log manual");
    memStateActive = true;
}



void setFunctionCallsFile(const char* path){
    if (funcCall){
        fclose(funcCall);
    }

    funcCall = fopen(path, "w");
    assert(funcCall && "setting functioncalls log manual");
    funcCallActive = true;
    functionDepth = 0;
}


void setFileParsingFile(const char* path){
    if (fileParse){
        fclose(fileParse);
    }

    fileParse = fopen(path, "w");
    assert(fileParse && "setting fileparsing log manual");
    fileParseActive = true;
}


void logMessage(enum LoggingFileType whichFile, const char* fmt, ...){
    FILE* output;
    if (whichFile == OUT){
        output = stdout;
    } else if (whichFile == MEMORY_STATE){
        if (memStateActive == false) return;
        output = memState;
    } else if (whichFile == FUNCTION_CALLS){
        if (funcCallActive == false) return;
        output = funcCall;
    } else if (whichFile == FILE_PARSING){
        if (fileParseActive == false) return;
        output = fileParse;
    } else {
        puts("log message failure to find whichFile");
        fflush(stdout);
        exit(1);
    }

    va_list args;
    va_start(args, fmt);

    vfprintf(output, fmt, args);
    fflush(output);
    
    va_end(args);
}


void logFuncStart(enum LoggingFileType whichFile, const char* fmt, ...){
    FILE* output;
    if (whichFile == OUT){
        output = stdout;
    } else if (whichFile == MEMORY_STATE){
        if (memStateActive == false) return;
        output = memState;
    } else if (whichFile == FUNCTION_CALLS){
        if (funcCallActive == false) return;
        output = funcCall;
    } else if (whichFile == FILE_PARSING){
        if (fileParseActive == false) return;
        output = fileParse;
    } else {
        puts("log message failure to find whichFile");
        fflush(stdout);
        exit(1);
    }

    for(int i = 0; i < functionDepth; i++)
        fputc('\t', output);

    va_list args;
    va_start(args, fmt);

    vfprintf(output, fmt, args);
    fflush(output);
    
    va_end(args);

    functionDepth++;
}


void logFuncEnds(enum LoggingFileType whichFile, const char* fmt, ...){
    functionDepth--;

    FILE* output;
    if (whichFile == OUT){
        output = stdout;
    } else if (whichFile == MEMORY_STATE){
        if (memStateActive == false) return;
        output = memState;
    } else if (whichFile == FUNCTION_CALLS){
        if (funcCallActive == false) return;
        output = funcCall;
    } else if (whichFile == FILE_PARSING){
        if (fileParseActive == false) return;
        output = fileParse;
    } else {
        puts("log message failure to find whichFile");
        fflush(stdout);
        exit(1);
    }

    for(int i = 0; i < functionDepth; i++)
        fputc('\t', output);

    va_list args;
    va_start(args, fmt);

    vfprintf(output, fmt, args);
    fflush(output);
    
    va_end(args);
}



void flushLogs(){
    if (funcCallActive)
        fflush(funcCall);
    if (memStateActive)
        fflush(memState);
    if (fileParseActive)
        fflush(fileParse);
}


void logVariables(enum LoggingFileType whichFile, Storage* p_store, bool printPrebuiltFuncs){
    logMessage(whichFile, "Local Variables:\n");
    char* st;

    uint i;
    if (printPrebuiltFuncs){
        i = 0;
    } else {
        i = NUM_PREBUILTS_IDENTIFIERS;
    }
    for (; i < p_store->length; i++){
        if (p_store->identifiers[i].union_mode == NNPSTR_UNIONMODE_BUFFER){
            logMessage(whichFile, "\t%s : ", p_store->identifiers[i].string.buffer);
        } else {
            logMessage(whichFile, "\t%s : ", p_store->identifiers[i].string.dyn_str);
        }
        st = objValtoDynAllocStr(p_store->objs[i]);
        logMessage(whichFile, "%s\n", st);
        myFree(st);
    }
}