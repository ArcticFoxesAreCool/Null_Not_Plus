#ifndef KEYWORDSANDOPERATORS_H
#define KEYWORDSANDOPERATORS_H





extern const int NUM_KEYWORDS;

extern const char* KEYWORDS_ARR[];

extern const size_t KEYWORDS_SIZES[];


typedef enum {
    RARROW,
    N_RARROW,
    LARROW,
    N_LARROW,
    PLUS,
    N_PLUS,
    STAR,
    N_STAR,
    EQUAL,
    N_EQUAL,
    GREATER,
    LESSER,
    DOT
} Operator_e;

    // "->",// assignment and return
    // "~->",// typecast
    // "<-",// func and method call
    // "~<-",// class constructor 
    // "+",// add
    // "~+",// subtract
    // "*",// mult
    // "~*",// div
    // "=",// is equal to
    // "~=",// not equal
    // ">",// greater than
    // "<",// less than
    // "."// index and class var/method getting


extern const int NUM_OPERATORS;

extern const char* OPERATORS_ARR[];

extern const size_t OPERATORS_SIZES[];




#endif