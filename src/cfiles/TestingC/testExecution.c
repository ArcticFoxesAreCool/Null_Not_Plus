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



    {
        ObjArray useless = {.capacity = 0,.length = 0,.objs = NULL};
        condenseObjsAndOperators(&useless);
        
    }




    closeFile();
    freeTok_types();
    freeReader();
    deepFreeBigStorage();
}
