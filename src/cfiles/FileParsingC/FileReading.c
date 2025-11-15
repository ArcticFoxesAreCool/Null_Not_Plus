#include "../../headers/datatypes.h"


FILE* nnp_code = NULL;

Reader nian;



char* strnChOccur(const char* str, int n, char ch){
    for(int i = 0; i < n; i++){
        if (*(str + i) == ch){
            return (char*)str + i;
        } else if (*(str + i) == '\0'){
            return NULL;
        }
    }
    return NULL;
}



void openFile(const char* file_path){
    size_t len = strlen(file_path);
    if (len < 5){
        puts("Minimum file path length is 5.\nEx: f.nnp");// 5 
        exit(1);
    } else if ( strncmp(".nnp", file_path + (len - 4), 4 ) != 0){
        puts("Given path does not end with a file containing the extension '.nnp'");
        exit(1);
    }


    nnp_code = fopen(file_path, "r");
    if (nnp_code == NULL){
        puts("NNP file could not be found.\nTerminating interpertation.");
        exit(1);
    }
    logMessage(FILE_PARSING, "NNP File from path: %s, has been found and opened.\n", file_path);
}




void closeFile(){
    if (nnp_code){
        fclose(nnp_code);
        logMessage(FILE_PARSING, "NNP File has been closed.\n");
        nnp_code = NULL;
    } else {
        logMessage(FILE_PARSING, "Attempted to close NNP file while it was already closed\n");
        exit(1);
    }
}




void initReader(int size, int token_indicies_capacity){
    assert(size > 0 && token_indicies_capacity > 0);

    nian.charv = myMalloc(size);
    nian.token_indexes = myMalloc(token_indicies_capacity * sizeof(int));

    assert(nian.charv && nian.token_indexes);

    nian.sz = size;
    nian.tok_ind_capacity = token_indicies_capacity;
    nian.tok_ind_len = 0;
}






uint8_t readLine(){//returns 0 if eof read
    assert(nian.charv && nnp_code);

    if ( !(fgets(nian.charv, nian.sz, nnp_code)) ){
        return false;
    }
    

    while (strnChOccur(nian.charv, nian.sz, '\n') == NULL  && !(feof(nnp_code)) ){
        nian.charv = myRealloc(nian.charv, nian.sz * 2);
        assert(nian.charv);

        fgets(nian.charv + nian.sz - 1, nian.sz + 1, nnp_code);
        nian.sz *= 2;
    }

    logMessage(FILE_PARSING, "Line Read: %s\n", nian.charv);
    return true;
}


void freeReader(){

    assert(nian.charv && nian.token_indexes && "double free attempt on nian");
    
    myFree(nian.charv);
    myFree(nian.token_indexes);

    nian.charv = NULL;
    nian.token_indexes = NULL;

    nian.sz = 0;

    nian.tok_ind_len = 0;
    nian.tok_ind_capacity = 0;
}


void tokenTime(){

    assert(nian.charv && nian.token_indexes && nian.sz > 1 && nian.tok_ind_capacity > 0);

    nian.tok_ind_len = 0;
    if (nian.charv[0] == '\n') return;


    int ln_len = (int)strnlen(nian.charv, nian.sz);
    
    if (nian.charv[ln_len - 1] == '\n') {
        nian.charv[--ln_len] = '\0';
    }

    int starting_char;
    for(starting_char = 0; starting_char <= ln_len; starting_char++){
        if (nian.charv[starting_char] == '\0'){
            return;
        } else if (nian.charv[starting_char] != ' ' && nian.charv[starting_char] != '\t'){
            break;
        }
    }

    nian.token_indexes[nian.tok_ind_len++] = starting_char;


    bool is_string = false;

    // printf("%s\n", nian.charv + starting_char);
    for(int i = starting_char; i < ln_len; i++){
        // printf("[%d]%c ", i, nian.charv[i]);
        if (nian.charv[i] == '\"') is_string = !(is_string);

        if ( (nian.charv[i] == ' ' || nian.charv[i] == '\t') && !(is_string) ){
            nian.charv[i] = '\0';


            if (nian.tok_ind_capacity == nian.tok_ind_len){
                nian.tok_ind_capacity *= 2;
                nian.token_indexes = myRealloc(nian.token_indexes, sizeof(int) * nian.tok_ind_capacity);
                assert(nian.token_indexes);
            }
            // if (nian.token_indexes[nian.tok_ind_len + 1] )
            // i++;
            if (nian.charv[i+1] != '\0' && nian.charv[i+1] != ' ' && nian.charv[i+1] != '\t') nian.token_indexes[nian.tok_ind_len++] = i+1;
        }
    }
    // logMessage(FILE_PARSING, "\n");
    for (int i = 0; i < nian.tok_ind_len; i++){
        logMessage(FILE_PARSING, "   token[%d]: '%s'", i, nian.charv + nian.token_indexes[i]);
    }
    logMessage(FILE_PARSING, "\n");
    // logMessage(FILE_PARSING, "\n\n");
}


