#include "../headers/datatypes.h"



int main(int argc, char** argv){

    initLogging();

    if (argc == 2){
        executeCode(argv[1]);
    } else {
        puts("Provide a NNP file path");
    }



    closeLogging();
 
    
}

