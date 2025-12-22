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
long tell = 0;
struct BlockTracker block_tracker = {0};
// struct ConditionalTracking conditional_tracker = {0};
// struct LoopList loop_list = {0};


static bool checkShouldResumeExecute(enum TypeOfLine line_type);

static void executeTheLine(ObjArray* p_line_memory, enum TypeOfLine line_type);

static void freeNonVarsInObjArr(ObjArray* p_obj_arr, const Storage* p_store);


void executeCode(const char* nnp_path){
    initBigStorage(32);
    initReader(READER_INITIAL_SIZE, READER_TOKEN_ARR_INITIAL_CAPACITY);
    initTok_types(INITIAIL_TOKEN_TYPER_SIZE);
    openFile(nnp_path);
    initBlockTracker(&block_tracker, BLOCK_TRACKER_INITIAL_CAPACITY);
    // initConditionalTracker(&conditional_tracker, 8);
    // initLoopList(&loop_list, 4);

    extern FILE* nnp_code;

    extern Reader nian;
    extern TokenTyper tok_types;

    ObjArray line_memory = {.capacity = 16,.length = 0,.objs = myMalloc(sizeof(object_p) * 16)};
    assert(line_memory.objs);

    enum TypeOfLine current_line_type;
    // struct ScopeTracker scope_info = {.execution_level = 0, .no_nest_level = 0};

    tell = ftell(nnp_code);

    while (readLine()){
        // printf("tell: %ld\n", tell);
        line_number++;

        tokenTime();
        getTok_types();


        current_line_type = getCurrentLineType(&big_storage);

        executeTheLine(&line_memory, current_line_type);
    
        freeNonVarsInObjArr(&line_memory, &big_storage);

        endOfLineLogging();

        tell = ftell(nnp_code);
    }
    // printf("\nConditionalTracker, length: %d\n", conditional_tracker.length);

    // freeLoopList(&loop_list);
    // freeConditionalTracker(&conditional_tracker);    
    freeBlockTracker(&block_tracker);
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
    // logMessage(MEMORY_STATE, "No Scope: %d\tSkip Execution: %d\n", no_scope, skip_execution);
    // for (int i = 0; i < block_tracker.length; i++){
    //     logMessage(MEMORY_STATE, "block(%d)=%d\n", i, block_tracker.data[i].state);
    // }
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








/*
no scope: 0
skip: false

scope tracker:
    if 0 (add to block tracker, no scope, )
        loop 1 (if false, set skip to true)
            if 2
            no 2 (clear previous)
        no 1 (if loop exit clear previous return to loop, else clear previous)
    no if 0 (if has triggered then set skip to true, else clear previous then start new if)
    no now 0 (if has triggered then set skip to true, else clear previous then start new now)
    no 0 (clear previous)


    if 0
    no now 0
    no 0
no 0
*/



static bool checkShouldResumeExecute(enum TypeOfLine line_type){
    assert(skip_execution);

    extern Reader nian;
    extern TokenTyper tok_types;
    
    if (nian.tok_ind_len == 0 || line_type == LINE_ARITHMETIC) return false;

    switch (line_type){
    case LINE_LOOP:
        if (loopLineIsBreakContinue() != LOOP_START) return false;    
    
    case LINE_CONDITIONAL:
    case LINE_CLASS_DECLARATION:
    case LINE_FUNC_DECLARATION:
        no_scope++;
    default:
        break;
    }
    if (strncmp(nian.charv + nian.token_indexes[0], "no", 3) == 0){
        no_scope--;
    } 

    return (no_scope == block_tracker.length - 1) || 
        (no_scope == block_tracker.length && (line_type == LINE_CONDITIONAL));

}






/*
no scope: 0
skip: false

scope tracker:
    if 0 (add to block tracker, no scope, )
        loop 1 (if false, set skip to true)
            if 2
            no 2 (clear previous)
        no 1 (if loop exit clear previous return to loop, else clear previous)
    no if 0 (if has triggered then set skip to true, else clear previous then start new if)
    no now 0 (if has triggered then set skip to true, else clear previous then start new now)
    no 0 (clear previous)


    if 0
    no now 0
    no 0
no 0
*/
// static int popCount = 0, appendCount = 0;



static void executeTheLine(ObjArray* p_line_memory, enum TypeOfLine line_type){
    extern Reader nian;
    extern TokenTyper tok_types;
    assert(nian.charv && nian.token_indexes);
    assert(tok_types.types);
    assert(p_line_memory && p_line_memory->objs && p_line_memory->capacity >= 1);

    bool can_decrement_no = true;
    if (skip_execution){
        skip_execution = !checkShouldResumeExecute(line_type);
        can_decrement_no = line_type != LINE_NO_SCOPE;
    }
    if (skip_execution) return;

    switch(line_type){
    case LINE_ARITHMETIC:
        condenseObjsAndOperators(p_line_memory);
        break;
    case LINE_CONDITIONAL:
        assert(nian.tok_ind_len >= 2);
        if (strncmp(nian.charv + nian.token_indexes[0], "if", 3) == 0){
            no_scope++;
            subCondenseObjsOperators(p_line_memory, NULL, 1, nian.tok_ind_len - 1);
            if (!(p_line_memory->length == 1 && *((Datatype_e*)(p_line_memory->objs[0])) == BOOL_OBJ)) {
                logMessage(OUT, "Error, if's expression does not evaluate to one BoolObj");
                exit(1);
            } else if (((BoolObj*)(p_line_memory->objs[0]))->value == true){
                appendBlockTracker(&block_tracker, (struct BlockData){.state = BLOCK_IF_HAS_TRIGGERED});
            } else {
                appendBlockTracker(&block_tracker, (struct BlockData){.state = BLOCK_IF_NOT_TRIGGERED});
                skip_execution = true;
            }
        } else if (strncmp(nian.charv + nian.token_indexes[1], "if", 3) == 0){
            assert(block_tracker.length >= 1);
            // no_scope++;
            if (block_tracker.data[block_tracker.length - 1].state == BLOCK_IF_HAS_TRIGGERED){
                skip_execution = true;
                // no_scope--;
            } else if (block_tracker.data[block_tracker.length - 1].state == BLOCK_IF_NOT_TRIGGERED){
                subCondenseObjsOperators(p_line_memory, NULL, 2, nian.tok_ind_len - 1);
                if (!(p_line_memory->length == 1 && *((Datatype_e*)(p_line_memory->objs[0])) == BOOL_OBJ)) {
                    logMessage(OUT, "Error, no if's expression does not evaluate to one BoolObj");
                    exit(1);
                } else if (((BoolObj*)(p_line_memory->objs[0]))->value == true){
                    block_tracker.data[block_tracker.length - 1].state = BLOCK_IF_HAS_TRIGGERED;
                } else {
                    skip_execution = true;
                    // no_scope--;
                }
            }
        } else if (strncmp(nian.charv + nian.token_indexes[1], "now", 4) == 0){
            assert(block_tracker.length >= 1);
            // no_scope++;
            if (block_tracker.data[block_tracker.length - 1].state == BLOCK_IF_HAS_TRIGGERED){
                block_tracker.data[block_tracker.length - 1].state = BLOCK_NOW_NOT_EXECUTE;
                skip_execution = true;
                // no_scope--;
            } else if (block_tracker.data[block_tracker.length - 1].state == BLOCK_IF_NOT_TRIGGERED){
                block_tracker.data[block_tracker.length - 1].state = BLOCK_NOW_WILL_EXECUTE;
                // no_scope++;
            }
        }
        break;
    case LINE_LOOP:
        assert(nian.tok_ind_len >= 2);
        // printf("\nappendcount: %d\tlength: %d\n", ++appendCount, block_tracker.length);

        enum LoopBreakContinue loop_type = loopLineIsBreakContinue();

        switch (loop_type){

        case LOOP_BREAK:
            for (int i = block_tracker.length - 1; i >= 0; i--){
                if (block_tracker.data[i].state == BLOCK_LOOP_REEXECUTE){
                    block_tracker.data[i].state = BLOCK_LOOP_LEAVE;
                    skip_execution = true;
                    goto outOfLoopSwitch;
                }
            }
            assert("ExecutionManager.c executeTheLine case LOOP_BREAK" && false);
        case LOOP_CONTINUE:
            for (int i = block_tracker.length - 1; i >= 0; i--){
                if (block_tracker.data[i].state == BLOCK_LOOP_REEXECUTE){
                    skip_execution = true;
                    goto outOfLoopSwitch;
                }
            }
            assert("ExecutionManager.c executeTheLine case LOOP_BREAK" && false);

        case LOOP_START:
            no_scope++;
            subCondenseObjsOperators(p_line_memory, NULL, 1, nian.tok_ind_len - 1);
            if (!(p_line_memory->length == 1 && *((Datatype_e*)(p_line_memory->objs[0])) == BOOL_OBJ)) {
                logMessage(OUT, "Error, loop's expression does not evaluate to one BoolObj");
                exit(1);
            } else if (((BoolObj*)(p_line_memory->objs[0]))->value == true){
                appendBlockTracker(&block_tracker, (struct BlockData){.loop_tell = tell, .state = BLOCK_LOOP_REEXECUTE, .line_count = line_number - 1});
            } else {
                // printf("\nblocktracker.len 1: %d", block_tracker.length);
                appendBlockTracker(&block_tracker, (struct BlockData){.state = BLOCK_LOOP_LEAVE});
                // printf("\nblocktracker.len 2: %d\n\n", block_tracker.length);
                skip_execution = true;
            }
            break;
        }
        outOfLoopSwitch:

        break;
        
    case LINE_CLASS_DECLARATION:
    case LINE_FUNC_DECLARATION:
        puts("Class and Function Declaration unimplemented. Try again in a few weeks");
        exit(1);
        break;
    case LINE_NO_SCOPE:
        if (can_decrement_no) no_scope--;
        // puts("SDFKLSJFDS:LKDFJ");fflush(stdout);
        assert(block_tracker.length >= 1);
        if (block_tracker.data[block_tracker.length - 1].state == BLOCK_LOOP_REEXECUTE){
            returnToLoop(&block_tracker);
        }
        popBlocktracker(&block_tracker);
        // printf("popcount: %d\n", ++popCount);
        break;
    case LINE_BLANK:
        break;
    }
}