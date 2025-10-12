
#include "../../headers/TestingH/testStoring.h"



void objArrFuncsTest(){
    ObjArray arr = {
        .capacity = 32,
        .length = 22,
        .objs = malloc(32 * sizeof(object_p))
    };
    assert(arr.objs);

    constructObjsTest(arr.objs);
    
    {
        BoolObj* temp = constructBoolObj(true);
        insertInObjArray(&arr, temp, 0);
        freeObj(temp);
    }

    char st[OBJ_PRINTING_CHAR_SIZE];
    for(uint i = 0; i < arr.length; i++){
        objValtoStr(st, arr.objs[i]);
        printf("arr[%u]: %s\n", i, st);
    }
    



    for(uint i = 0; i < arr.length; i++){
        freeObj(arr.objs[i]);
    }
    free(arr.objs);
}





void nnpsstrTest(){

    NnpStr strs[] = {
        makeNnpStr(""),
        makeNnpStr(""),
        makeNnpStr("abcdefg"),
        makeNnpStr("abcdefg"),
        makeNnpStr("longStrHere"),
        makeNnpStr("nnpstrcanbebig"),
        makeNnpStr("longStrHere"),
        makeNnpStr("a"),
        makeNnpStr("1"),
        makeNnpStr("1"),
    };




    for(size_t i = 0; i < sizeof(strs) / sizeof(NnpStr); i++){
        if(strs[i].union_mode == NNPSTR_UNIONMODE_BUFFER){
            printf("strs[%zu], B: %s\n", i, strs[i].string.buffer);
        } else {
            printf("strs[%zu], D: %s\n", i, strs[i].string.dyn_str);
        }
    }
    puts("\n");

    // // cmps with empty str
    // printf("[%d] == [%d]: %u\n",0,1, NnpStrEq(&(strs[0]), &(strs[1])) );
    // printf("[%d] == [%d]: %u\n",0,9, NnpStrEq(&(strs[0]), &(strs[9])) );
    // printf("[%d] == [%d]: %u\n",0,5, NnpStrEq(&(strs[0]), &(strs[5])) );
    // putchar('\n');
    // // cmps with buffer,
    // printf("[%d] == [%d]: %u\n",2,3, NnpStrEq(&(strs[2]), &(strs[3])) );
    // printf("[%d] == [%d]: %u\n",2,9, NnpStrEq(&(strs[2]), &(strs[9])) );
    // printf("[%d] == [%d]: %u\n",2,5, NnpStrEq(&(strs[2]), &(strs[5])) );
    // putchar('\n');

    // // cmps with dyn
    // printf("[%d] == [%d]: %u\n",4,6, NnpStrEq(&(strs[4]), &(strs[6])) );
    // printf("[%d] == [%d]: %u\n",4,5, NnpStrEq(&(strs[4]), &(strs[5])) );
    //NnpStrEq works!!!!

    // catNnpStrs(&(strs[0]), &(strs[0]));
    // if (strs[0].union_mode == NNPSTR_UNIONMODE_BUFFER){
    //     printf(":%s\n", strs[0].string.buffer);
    // } else {
    //     printf(":%s\n", strs[0].string.dyn_str);
    // }
    catNnpStrs(&(strs[7]), &(strs[8]));
    if (strs[7].union_mode == NNPSTR_UNIONMODE_BUFFER){
        printf(":%s\n", strs[7].string.buffer);
    } else {
        printf(":%s\n", strs[7].string.dyn_str);
    }


    for(size_t i = 0; i < sizeof(strs) / sizeof(NnpStr); i++){
        freeNnpStr(&(strs[i]));
    }
}


void storageFuncsTest(){

    Storage* pstore = malloc(sizeof(Storage));
    assert(pstore);

    pstore->capacity = 8;
    pstore->length = 3;
    pstore->objs = malloc(sizeof(object_p) * pstore->capacity);
    assert(pstore->objs);

    pstore->identifiers = malloc(sizeof(NnpStr) * pstore->capacity);
    assert(pstore->identifiers);

    int i = 0;
    pstore->objs[i] = constructBoolObj(true);
    pstore->identifiers[i++] = makeNnpStr("boo");

    pstore->objs[i] = constructDatatypeObj(NUM_OBJ);
    pstore->identifiers[i++] = makeNnpStr("dat");

    pstore->objs[i] = constructNumObj(M_PI);
    pstore->identifiers[i++] = makeNnpStr("pie");

    // pstore->objs[i] = constructStrObj_char("mah strin'!");
    // pstore->identifiers[i++] = makeNnpStr("thread");
    {
    NnpStr temp = makeNnpStr("thread");
    StrObj* s = constructStrObj_char("mah strin'!");



    // printf("datatype: %d, %d\n", s->type, *((Datatype_e*)s));
    appendStorage(pstore, s, &temp);
    freeObj(s);
    freeNnpStr(&temp);
    }


    // printf("name: %s\n", pstore->identifiers[3].string.buffer);
    

    char st[OBJ_PRINTING_CHAR_SIZE];

    NnpStr nstr = makeNnpStr("boo");

    // objValtoStr(st, getFromStorage(pstore, &nstr));
    // puts(st);
    // freeNnpStr(&nstr);

    // setNnpStr("dat", &nstr);

    // objValtoStr(st, getFromStorage(pstore, &nstr));
    // puts(st);
    // freeNnpStr(&nstr);

    setNnpStr("pie", &nstr);
    objValtoStr(st, getFromStorage(pstore, &nstr));
    puts(st);
    // freeNnpStr(&nstr);

    removeStorage(pstore, &nstr);
    printf("%u, %s\n", pstore->length, pstore->identifiers[2].string.buffer);
    
    // objValtoStr(st, getFromStorage(pstore, &nstr));
    // puts(st);




    setNnpStr("thread", &nstr);

    objValtoStr(st, getFromStorage(pstore, &nstr));
    puts(st);

    freeNnpStr(&nstr);





    freeStorage(pstore);
}