#include "../../headers/datatypes.h"

extern CallStack call_stack;
extern long tell;
extern int line_number;

void initCallStack(int capacity){
    assert(call_stack.capacity == 0 && call_stack.length == 0 && call_stack.local_vars == NULL && call_stack.where_will_return == NULL && call_stack.previous_line == NULL);

    call_stack.local_vars = myMalloc(sizeof(Storage) * capacity);
    assert(call_stack.local_vars);
    call_stack.where_will_return = myMalloc(sizeof(long) * capacity);
    assert(call_stack.where_will_return);
    call_stack.previous_line = myMalloc(sizeof(int) * capacity);
    assert(call_stack.previous_line);

    call_stack.capacity = capacity;
}

void freeCallStack(){
    assert(call_stack.capacity > 0 && call_stack.local_vars && call_stack.where_will_return && call_stack.previous_line);
    if (call_stack.length != 0){
        logMessage(OUT, "\nFree called on non-empty call stack\n");
        for (int i = 0; i < call_stack.length; i++){
            Storage storage_ref = call_stack.local_vars[i];
            for (uint j = 0; j < storage_ref.length; j++){
                freeObj(storage_ref.objs[j]);
                freeNnpStr(storage_ref.identifiers + j);
            }
            myFree(storage_ref.objs);
            myFree(storage_ref.identifiers);
        }
        call_stack.length = 0;
    } 
    
    myFree(call_stack.local_vars);
    call_stack.local_vars = NULL;
    myFree(call_stack.where_will_return);
    call_stack.where_will_return = NULL;
    myFree(call_stack.previous_line);
    call_stack.previous_line = NULL;

    call_stack.capacity = 0;
    

}

void appendCallStack(long tell, int previous_line){
    assert(call_stack.capacity > 0 && call_stack.local_vars && call_stack.where_will_return && call_stack.previous_line);
    assert(tell > 0);

    if (call_stack.length == call_stack.capacity){
        call_stack.capacity *= 2;
        call_stack.local_vars = myRealloc(call_stack.local_vars, sizeof(Storage) * call_stack.capacity);
        call_stack.where_will_return = myRealloc(call_stack.where_will_return, sizeof(long) * call_stack.capacity);
        call_stack.previous_line = myRealloc(call_stack.previous_line, sizeof(int) * call_stack.capacity);

        assert(call_stack.local_vars && call_stack.where_will_return && call_stack.previous_line);
    }
    call_stack.local_vars[call_stack.length] = (Storage){0};
    initStorage(&(call_stack.local_vars[call_stack.length]), STORAGE_INITIAL_CAPACITY);
    call_stack.where_will_return[call_stack.length] = tell;
    call_stack.previous_line[call_stack.length] = previous_line;

    call_stack.length++;
}

void popAndFSeekCallStack(){
    assert(call_stack.capacity > 0 && call_stack.length > 0 && call_stack.local_vars && call_stack.where_will_return && call_stack.previous_line);

    extern FILE* nnp_code;

    call_stack.length--;
    {
        Storage stor_ref = call_stack.local_vars[call_stack.length];
        for(uint i = 0; i < stor_ref.length; i++){
            freeObj(stor_ref.objs[i]);
            freeNnpStr(stor_ref.identifiers + i);
        }
        myFree(stor_ref.objs);
        myFree(stor_ref.identifiers);
    }
    fseek(nnp_code, call_stack.where_will_return[call_stack.length], SEEK_SET);
    line_number = call_stack.previous_line[call_stack.length];

    // printf("CallStack Popped: %p\n", &(call_stack.local_vars[call_stack.length]));fflush(stdout);
}


static void setParameters(const ObjArray* p_obj_arr, Storage* p_store, int num_args);


void resolveUserFunc(ObjArray* p_obj_arr, int num_args){
    extern FILE* nnp_code;
    extern struct BlockTracker block_tracker;
    extern int no_scope;

    assert(nnp_code && no_scope >= 0);
    assert(p_obj_arr && p_obj_arr->capacity >= (uint)num_args + 1 && p_obj_arr->length >= (uint)num_args + 1 && p_obj_arr->objs);
    assert(num_args >= 0);

    appendCallStack(tell, line_number);
    appendBlockTracker(&block_tracker, (struct BlockData){.state = BLOCK_FUNCTION, .line_count = line_number - 1});

    
    UserFuncObj* func_ref = p_obj_arr->objs[p_obj_arr->length - num_args - 1];
    int initial_no_scope = no_scope;
    no_scope++;


    ObjArray line_memory = {.capacity = 16,.length = 0,.objs = myMalloc(sizeof(object_p) * 16)};
    assert(line_memory.objs);

   
    logMessage(MEMORY_STATE, "User Function Executing:\n");
    fseek(nnp_code, func_ref->declaration_tell, SEEK_SET);
    line_number = func_ref->line_number;

    readLine();
    tokenTime();

    setParameters(p_obj_arr, &(call_stack.local_vars[call_stack.length - 1]), num_args);
    
    enum TypeOfLine line_type;
    object_p ret = NULL;

    tell = ftell(nnp_code);
    while (no_scope > initial_no_scope && readLine()){
        line_number++;
        tokenTime();
        getTok_types();

        line_type = getCurrentLineType(&(call_stack.local_vars[call_stack.length - 1]));

        if (line_type == LINE_RETURN){
            ret = getReturnValue(&(call_stack.local_vars[call_stack.length - 1]), &line_memory);
            if (ret){
                appendInObjArray(p_obj_arr, ret);
            }

            freeNonVarsInObjArr(&line_memory, &(call_stack.local_vars[call_stack.length - 1]));
            endOfLineLogging(&(call_stack.local_vars[call_stack.length - 1]), false);

            // while (block_tracker.data[block_tracker.length - 1].state != BLOCK_FUNCTION){
            //     popBlocktracker(&block_tracker);
            // }
            popBlocktracker(&block_tracker);

            break;
        }

        executeTheLine(&line_memory, line_type, &(call_stack.local_vars[call_stack.length - 1]));
        freeNonVarsInObjArr(&line_memory, &(call_stack.local_vars[call_stack.length - 1]));
        endOfLineLogging(&(call_stack.local_vars[call_stack.length - 1]), false);

        tell = ftell(nnp_code);


    }

    myFree(line_memory.objs);
    /*
    BECAUSE THERE ARE NO RETURNS #todo, JUST CLEAR ARGS and USERFUNC_OBJ
    */
    int index_offset = ret ? -2 : -1;
    for (int i = 0; i < num_args + 1; i++){
        // printf("len: %u\n", p_obj_arr->length); fflush(stdout);
        popInObjArray(p_obj_arr, p_obj_arr->length + index_offset);
    }

    no_scope = initial_no_scope;
    popAndFSeekCallStack();
    readLine();
    tokenTime();
    getTok_types();
}

static void setParameters(const ObjArray* p_obj_arr, Storage* p_store, int num_args){
    extern Reader nian;
    assert(nian.charv && nian.sz > 0 && nian.tok_ind_capacity > 0 && nian.tok_ind_len >= 2);

    assert(p_obj_arr->length >= (uint)(1 + num_args));

    

    if (num_args < 1) return;

    NnpStr parameter_name = {0};

    int j = 2;
    for (uint i = p_obj_arr->length - num_args; i < p_obj_arr->length && j < nian.tok_ind_len; i++, j++){
        setNnpStr(nian.charv + nian.token_indexes[j], &parameter_name);
        appendStorage(p_store, p_obj_arr->objs[i], &parameter_name);
    }
    freeNnpStr(&parameter_name);


}


object_p getReturnValue(Storage* p_store, ObjArray* p_obj_array){
    extern Reader nian;
    assert(p_store && p_store->capacity > 0 && p_store->length > 0 && p_store->objs && p_store->identifiers);
    assert(nian.charv && nian.sz > 0 && nian.tok_ind_len > 0 && nian.tok_ind_capacity > 0 && nian.token_indexes);
    assert(strncmp("->", nian.charv + nian.token_indexes[nian.tok_ind_len - 1], 3) == 0);
    
    if (nian.tok_ind_len == 1){
        return NULL;
    }
    subCondenseObjsOperators(p_store, p_obj_array, NULL, 0, nian.tok_ind_len - 2);
    if (p_obj_array->length > 0){
        return p_obj_array->objs[p_obj_array->length - 1];
    }
    return NULL;
}