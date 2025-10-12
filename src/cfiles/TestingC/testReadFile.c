#include "../../headers/TestingH/testReadFile.h"


void testFile(){
    extern FILE* nnp_code;
    printf("nnp_code: %p\n", nnp_code);

    openFile("./ExampleNNPfiles/testIfOpen.nnp");
    // nonexistant file:

    // openFile("./ExampleNNpfiles/notrealfile.nnp"); error, :)
    // openFile("a.nnp"); openFile works excently


    printf("nnp_code: %p\n", nnp_code);

    closeFile();
    // closeFile(); all good!
    printf("nnp_code: %p\n", nnp_code);

}

void testReader(){
    extern Reader nian;

    // initReader(READER_INITIAL_SIZE, READER_TOKEN_ARR_INITIAL_CAPACITY);
    initReader(READER_INITIAL_SIZE, READER_TOKEN_ARR_INITIAL_CAPACITY);



    openFile("./ExampleNNPfiles/testIfOpen.nnp");
    
    for(int i = 0; readLine(); i++){
        printf("tokenization attempt line %d:\n", i+1);
        tokenTime();

        if (nian.tok_ind_len == 0){
            puts("\tBlank Line\n"); continue;
        }

        for(int j = 0; j < nian.tok_ind_len; j++){
            printf("\tToken[%d]: len= %lu, '%s'\n", j, strlen(nian.charv+nian.token_indexes[j]), nian.charv+nian.token_indexes[j]);
        }
        puts("\n");
        // printf("rsize: %d, lnsize: %lu, In File line %d: '%s'\n", nian.sz, strlen(nian.charv)+1, i+1, nian.charv);
    }
    closeFile();


    putchar('\n');
    printf("%d, %d, %d\n", nian.sz, nian.tok_ind_capacity, nian.tok_ind_len);
    freeReader();
    // freeReader(); double free error
}
