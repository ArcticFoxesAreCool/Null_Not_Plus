#include "../../headers/TestingH/testExecution.h"
#include "../../headers/datatypes.h"



void executionTest(){
    initBigStorage(1);
    initReader(READER_INITIAL_SIZE, READER_TOKEN_ARR_INITIAL_CAPACITY);
    initTok_types(INITIAIL_TOKEN_TYPER_SIZE);
    openFile("./ExampleNNPfiles/testCondense.nnp");

    extern Reader nian;
    extern TokenTyper tok_types;


    readLine();
    tokenTime();
    getTok_types();
    puts("Tokens:");
    for(int i = 0; i < nian.tok_ind_len; i++) printf("  '%s'", nian.charv + nian.token_indexes[i]);
    putchar('\n');


    object_p val1 = constructFromValue(0), val2 = constructFromValue(2);
    // char val_to_str[OBJ_PRINTING_CHAR_SIZE];
    // objValtoStr(val_to_str, val1); puts(val_to_str);
    // objValtoStr(val_to_str, val2); puts(val_to_str);

    {
        ObjArray useless = {.capacity = 0,.length = 0,.objs = 0};

        condenseObjsAndOperators(&useless);
        free(useless.objs);
    }





    freeObj(val1); val1 = NULL;
    freeObj(val2); val2 = NULL;


    closeFile();
    freeTok_types();
    freeReader();
    deepFreeBigStorage();
}
