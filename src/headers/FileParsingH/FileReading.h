#ifndef FILEREADING_H
#define FILEREADING_H


typedef struct {
    char* charv;
    int sz;


    int* token_indexes;
    int tok_ind_len;
    int tok_ind_capacity;
} Reader;


void openFile(const char* file_path);
void closeFile();


#define READER_INITIAL_SIZE 256
#define READER_TOKEN_ARR_INITIAL_CAPACITY 8

void initReader(int size, int token_indicies_capacity);
uint8_t readLine();
void freeReader();
void tokenTime();

char* strnChOccur(const char* str, int n, char ch);


#endif