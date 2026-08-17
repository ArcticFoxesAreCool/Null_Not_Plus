#include "../../headers/datatypes.h"


extern CallStack call_stack;
extern long tell;
extern int line_number;

InstanceObj* me_obj = NULL;

void resolveClassConstructor(ObjArray* p_obj_arr, int num_args){
    extern FILE* nnp_code;
    extern struct BlockTracker block_tracker;
    extern int no_scope;
    extern bool skip_execution;

    assert(nnp_code && no_scope >= 0);
    assert(p_obj_arr && p_obj_arr->capacity >= (uint)num_args + 1 && p_obj_arr->length >= (uint)num_args + 1 && p_obj_arr->objs);
    assert(num_args >= 0);

    appendCallStack(tell, line_number);
    appendBlockTracker(&block_tracker, (struct BlockData){.state = BLOCK_FUNCTION, .line_count = line_number});

    
    ClassObj* class_ref = p_obj_arr->objs[p_obj_arr->length - num_args - 1];
    int initial_no_scope = no_scope;
    no_scope++;

    object_p instance = constructInstanceObj(class_ref);
    NnpStr inst_name = makeNnpStr("me");

    appendStorage(&(call_stack.local_vars[call_stack.length - 1]), instance, &inst_name);

    freeObj(instance);


    ObjArray line_memory = {.capacity = 16,.length = 0,.objs = myMalloc(sizeof(object_p) * 16)};
    assert(line_memory.objs);

    me_obj = getFromStorage(&(call_stack.local_vars[call_stack.length - 1]), &inst_name);
   
    logMessage(MEMORY_STATE, "Class Constructor Executing:\n");
    fseek(nnp_code, class_ref->constructor->declaration_tell, SEEK_SET);
    line_number = class_ref->constructor->line_number;

    readLine();
    tokenTime();

    setParameters(p_obj_arr, &(call_stack.local_vars[call_stack.length - 1]), num_args);
    
    enum TypeOfLine line_type;

    tell = ftell(nnp_code);
    while (no_scope > initial_no_scope && readLine()){
        line_number++;
        tokenTime();
        getTok_types();

        line_type = getCurrentLineType(&(call_stack.local_vars[call_stack.length - 1]));

        if (line_type == LINE_RETURN && !skip_execution){
            logMessage(FILE_PARSING, "Class constructors do not have return statements. Line: %d\n", line_number);
            exit(1);
        }



        executeTheLine(&line_memory, line_type, &(call_stack.local_vars[call_stack.length - 1]));
        freeNonVarsInObjArr(&line_memory, &(call_stack.local_vars[call_stack.length - 1]));
        endOfLineLogging(&(call_stack.local_vars[call_stack.length - 1]), false);

        tell = ftell(nnp_code);


    }


    
    
    
    int index_offset = -1;
    for (int i = 0; i < num_args + 1; i++){
        // printf("len: %u\n", p_obj_arr->length); fflush(stdout);
        popInObjArray(p_obj_arr, p_obj_arr->length + index_offset);
    }

    appendInObjArray(p_obj_arr, me_obj);

    me_obj = NULL;
    myFree(line_memory.objs);
    freeNnpStr(&inst_name);


    no_scope = initial_no_scope;
    popAndFSeekCallStack();
    readLine();
    tokenTime();
    getTok_types();
}
