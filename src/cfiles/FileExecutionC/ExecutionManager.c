#include "../../headers/datatypes.h"


Storage big_storage = {
    .capacity = 0,
    .length = 0,
    .identifiers = NULL,
    .objs = NULL
};

int line_number = 0;

bool skip_execution = false;
int no_scope = 0;
bool if_has_been_triggered = false;


static bool checkShouldResumeExecute(enum TypeOfLine line_type);

static void executeTheLine(ObjArray* p_line_memory, enum TypeOfLine line_type);

static void freeNonVarsInObjArr(ObjArray* p_obj_arr, const Storage* p_store);


void executeCode(const char* nnp_path){
    initBigStorage(32);
    initReader(READER_INITIAL_SIZE, READER_TOKEN_ARR_INITIAL_CAPACITY);
    initTok_types(INITIAIL_TOKEN_TYPER_SIZE);
    openFile(nnp_path);

    extern Reader nian;
    extern TokenTyper tok_types;

    ObjArray line_memory = {.capacity = 16,.length = 0,.objs = myMalloc(sizeof(object_p) * 16)};
    assert(line_memory.objs);

    enum TypeOfLine current_line_type;
    // struct ScopeTracker scope_info = {.execution_level = 0, .no_nest_level = 0};


    while (readLine()){
        line_number++;

        tokenTime();
        getTok_types();


        current_line_type = getCurrentLineType(&big_storage);

        executeTheLine(&line_memory, current_line_type);
    
        freeNonVarsInObjArr(&line_memory, &big_storage);

        endOfLineLogging();
    }

    
    myFree(line_memory.objs);
    closeFile();
    freeTok_types();
    freeReader();
    deepFreeBigStorage();

    logAllocations(MEMORY_STATE);
}










void initBigStorage(int size){
    assert(size > 0 && big_storage.identifiers == NULL && big_storage.capacity == 0 && big_storage.length == 0 && big_storage.objs == NULL);
    big_storage.objs = myMalloc(sizeof(object_p) * size);
    big_storage.identifiers = myMalloc(sizeof(NnpStr) * size);
    assert(big_storage.objs && big_storage.identifiers);

    big_storage.capacity = size;
    big_storage.length = 0;

    loadPrebuiltsIntoStorage(&big_storage);
}





void deepFreeBigStorage(){
    assert(big_storage.objs && big_storage.identifiers && big_storage.capacity > 0);

    for(uint i = 0; i < big_storage.length; i++){
        freeObj(big_storage.objs[i]);
        freeNnpStr(  &(big_storage.identifiers[i])  );
    }

    myFree(big_storage.objs);
    myFree(big_storage.identifiers);

    big_storage.capacity = 0;
    big_storage.length = 0;
}


static void freeNonVarsInObjArr(ObjArray* p_obj_arr, const Storage* p_store){
    // printf("length: %u\n\tObj[0]: %p\n", p_obj_arr->length, p_obj_arr->objs[0]);fflush(stdout);
    while(p_obj_arr->length > 0){
        for (uint i = 0; i < p_store->length; i++){
            if (p_obj_arr->objs[p_obj_arr->length - 1] == p_store->objs[i]){
                p_obj_arr->length--;
                break;
            }
        }
        // printf("TEST\n");
        freeObj(p_obj_arr->objs[p_obj_arr->length - 1]);
        p_obj_arr->length--;
    }
}


void endOfLineLogging(){
    logMessage(FILE_PARSING, "\n\n");
    logMessage(FUNCTION_CALLS, "Finished line %d\n\n", line_number);

    logVariables(MEMORY_STATE, &big_storage, false);
    logMessage(MEMORY_STATE, "Finished line %d\n\n", line_number);
}


enum TypeOfLine getCurrentLineType(const Storage* p_store){
    extern Reader nian;
    extern TokenTyper tok_types;
    assert(nian.charv && nian.sz >= 1 && nian.tok_ind_capacity >= 1 && nian.token_indexes);
    assert(tok_types.size >= 1 && tok_types.types);

    if (nian.tok_ind_len == 0){
        return LINE_BLANK;
    } else if (nian.tok_ind_len == 1 && strncmp(nian.charv + nian.token_indexes[0], "no", 3) == 0) {
        return LINE_NO_SCOPE;
    } else if (nian.tok_ind_len >= 2 && strncmp(nian.charv + nian.token_indexes[1], "~<-", 4) == 0){
        return LINE_CLASS_DECLARATION;
    }

    
    if (strncmp(nian.charv + nian.token_indexes[0], "loop", 5) == 0){
        return LINE_LOOP;
    } else if (strncmp(nian.charv + nian.token_indexes[0], "if", 3) == 0){
        return LINE_CONDITIONAL;
    }

    if (nian.tok_ind_len >= 2){
        if (strncmp(nian.charv + nian.token_indexes[1], "if", 3) == 0 || strncmp(nian.charv + nian.token_indexes[1], "now", 4) == 0){
            return LINE_CONDITIONAL;
        } else if (strncmp(nian.charv + nian.token_indexes[1], "loop", 5) == 0){
            return LINE_LOOP;
        } else if (tok_types.types[0] == VARIABLE && strncmp(nian.charv + nian.token_indexes[1], "<-", 3) == 0){
            NnpStr temp_str = makeNnpStr(nian.charv + nian.token_indexes[0]);
            if (getFromStorage(p_store, &temp_str) == NULL){
                freeNnpStr(&temp_str);
                return LINE_FUNC_DECLARATION;
            }
            freeNnpStr(&temp_str);
        }
    }

    return LINE_ARITHMETIC;
}



static bool checkShouldResumeExecute(enum TypeOfLine line_type){
    assert(skip_execution);

    extern Reader nian;
    extern TokenTyper tok_types;

    switch(line_type){
    case LINE_LOOP:
        no_scope++;
        break;
    case LINE_NO_SCOPE:
        no_scope--;
        break;
    case LINE_CONDITIONAL:
        if (strncmp(nian.charv + nian.token_indexes[0], "if", 3) == 0){
            no_scope++;
        } else if (no_scope == 1){
            if (nian.tok_ind_len == 2 && !if_has_been_triggered && strncmp(nian.charv + nian.token_indexes[1], "now", 4) == 0){
                no_scope--;
                return true;
            } else if (nian.tok_ind_len >= 3 && !if_has_been_triggered && strncmp(nian.charv + nian.token_indexes[1], "if", 3) == 0){
                no_scope--;
                return true;
            }
        }
        break;
    default:
        return false;
    }

    return no_scope == 0;
}




static void executeTheLine(ObjArray* p_line_memory, enum TypeOfLine line_type){
    extern Reader nian;
    extern TokenTyper tok_types;
    assert(nian.charv && nian.token_indexes);
    assert(tok_types.types);
    assert(p_line_memory && p_line_memory->objs && p_line_memory->capacity >= 1);

    if (skip_execution) skip_execution = !checkShouldResumeExecute(line_type);
    if (skip_execution) return;

    switch(line_type){
    case LINE_ARITHMETIC:
        condenseObjsAndOperators(p_line_memory);
        break;
    case LINE_CONDITIONAL:
        assert(nian.tok_ind_len >= 2);
        if (strncmp(nian.charv + nian.token_indexes[0], "if", 3) == 0){
            if_has_been_triggered = false;
            subCondenseObjsOperators(p_line_memory, NULL, 1, nian.tok_ind_len - 1);
            if (!(p_line_memory->length == 1 && *((Datatype_e*)(p_line_memory->objs[0])) == BOOL_OBJ)) {
                logMessage(OUT, "Error, if's expression does not evaluate to one BoolObj");
                exit(1);
            } else if (((BoolObj*)(p_line_memory->objs[0]))->value != false){
                if_has_been_triggered = true;
            } else {
                no_scope++;
                skip_execution = true;
            }
        } else if (strncmp(nian.charv + nian.token_indexes[1], "if", 3) == 0){
            if (if_has_been_triggered){
                no_scope++;
                skip_execution = true;
            } else {
                subCondenseObjsOperators(p_line_memory, NULL, 1, nian.tok_ind_len - 1);
                if (!(p_line_memory->length == 1 && *((Datatype_e*)(p_line_memory->objs[0])) == BOOL_OBJ)) {
                    logMessage(OUT, "Error, no if's expression does not evaluate to one BoolObj");
                    exit(1);
                } else if (((BoolObj*)(p_line_memory->objs[0]))->value != false){
                    if_has_been_triggered = true;
                } else {
                    no_scope++;
                    skip_execution = true;
                }
            }
        } else if (if_has_been_triggered && strncmp(nian.charv + nian.token_indexes[1], "now", 4) == 0){
            no_scope++;
            skip_execution = true;
        }

        break;
    case LINE_LOOP:
        puts("Loops unimplemented yet");
        exit(1);
        break;
        
    case LINE_CLASS_DECLARATION:
    case LINE_FUNC_DECLARATION:
        puts("Class and Function Declaration unimplemented. Try again in a few weeks");
        exit(1);
        break;
    case LINE_BLANK:
    case LINE_NO_SCOPE:
        return;
    }
}