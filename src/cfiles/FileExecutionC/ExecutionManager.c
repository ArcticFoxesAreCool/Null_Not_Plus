#include "../../headers/datatypes.h"


Storage big_storage = {
    .capacity = 0,
    .length = 0,
    .identifiers = NULL,
    .objs = NULL
};

int line_number = 0;

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


    while (readLine()){
        line_number++;

        tokenTime();
        getTok_types();

        condenseObjsAndOperators(&line_memory);   
    
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

    logVariables(MEMORY_STATE, &big_storage, true);
    logMessage(MEMORY_STATE, "Finished line %d\n\n", line_number);
}