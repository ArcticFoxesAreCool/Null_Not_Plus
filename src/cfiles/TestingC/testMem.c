
#include "../../headers/TestingH/testMem.h"




void constructObjsTest(object_p* objArr){
    assert(objArr);

    int i = 0;

    objArr[i++] = constructBoolObj(0);
    objArr[i++] = constructBoolObj(1);
    objArr[i++] = constructBoolObj(255);// 3
    logMessage(FUNCTION_CALLS, "\n");
    objArr[i++] = constructDatatypeObj(BOOL_OBJ);
    objArr[i++] = constructDatatypeObj(DATATYPE_OBJ);
    objArr[i++] = constructDatatypeObj(LIST_OBJ);
    objArr[i++] = constructDatatypeObj(NUM_OBJ);
    objArr[i++] = constructDatatypeObj(STR_OBJ);
    objArr[i++] = constructDatatypeObj(FUNC_OBJ);
    objArr[i++] = constructDatatypeObj(CLASS_OBJ);
    objArr[i++] = constructDatatypeObj(INSTANCE_OBJ);// 8
    logMessage(FUNCTION_CALLS, "\n");

    objArr[i++] = constructNumObj(0.0);
    objArr[i++] = constructNumObj(1.0);
    objArr[i++] = constructNumObj(-1.0);
    objArr[i++] = constructNumObj(0.5);
    objArr[i++] = constructNumObj(-0.5);// 5
    logMessage(FUNCTION_CALLS, "\n");

    objArr[i++] = constructStrObj_char("short");
    objArr[i++] = constructStrObj_char("wow super long string");
    objArr[i++] = constructStrObj_char("");
    // objArr[i++] = constructStrObj_char(NULL);
    // 3
    logMessage(FUNCTION_CALLS, "\n");

    uint currLen = i;
    ObjArray tempBig = {
        .capacity = 32,
        .length = currLen,
        .objs = myMalloc(sizeof(object_p) * 32)
    };

    for(uint j = 0; j < currLen; j++){
        tempBig.objs[j] = copyObj(objArr[j]);
    }

    ObjArray tempEmpty = {
        .capacity = DEFAULT_OBJ_ARRAY_CAPACITY,
        .length = 0,
        .objs = myMalloc(sizeof(object_p) * DEFAULT_OBJ_ARRAY_CAPACITY)
    };

    ObjArray tempNull = {
        .capacity = DEFAULT_OBJ_ARRAY_CAPACITY,
        .length = 0,
        .objs = NULL
    };

    objArr[i++] = constructListObj(&tempEmpty);
    objArr[i++] = constructListObj(&tempNull);
    objArr[i++] = constructListObj(&tempBig);

    for(uint j = 0; j < currLen; j++){
        freeObj(tempBig.objs[j]);
    }
    myFree(tempBig.objs);

    myFree(tempEmpty.objs);
}











// void sleep(int delay){
//     struct timespec start_time, curr_time;
//     timespec_get(&start_time, TIME_UTC);
//     timespec_get(&curr_time, TIME_UTC);

//     while (curr_time.tv_sec - start_time.tv_sec < delay){
//         timespec_get(&curr_time, TIME_UTC);
//     }
// }




void memLeakTest(int n, int delay){
    // struct timespec tim;
    // timespec_get(&tim, TIME_UTC);
    logMessage(FUNCTION_CALLS, "\n\nSTART MEM LEAK TEST\n\n");


    ObjArray a = {
        .capacity = 32,
        .length = 22,
        .objs = myMalloc(sizeof(object_p) * 32)
    };
    assert(a.objs);
    constructObjsTest(a.objs);
    
    ListObj* lst = constructListObj(&a);

    for(uint i = 0; i < a.length; i++){
        freeObj(a.objs[i]);
    }
    myFree(a.objs);

    logMessage(FUNCTION_CALLS, "\n\nMEM LEAK: inital list done\n\n");

    for(int i = 0; i < n; i++){

        if (lst->values.capacity == lst->values.length){
            lst->values.capacity *= 2;
            lst->values.objs = myRealloc(lst->values.objs, sizeof(object_p) * lst->values.capacity);
            assert(lst->values.objs);
        }

        lst->values.objs[  lst->values.length  ] = copyObj(lst);
        lst->values.length += 1;
  
    }
    // printf("Length : %d\n", lst->values.length);


    logMessage(FUNCTION_CALLS, "\n\nMEM LEAK: Memory Made\n\n");


    puts("Delay Begin!");
    sleep(delay);
    puts("Freeing Memory!");


    freeObj(lst);

    logMessage(FUNCTION_CALLS, "\n\nMEM LEAK: Memory Freed\n\n");

}



