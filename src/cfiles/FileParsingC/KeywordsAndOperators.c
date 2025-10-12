#include "../../headers/datatypes.h"


const int NUM_KEYWORDS = 5;

const char* KEYWORDS_ARR[] = {
    "if",
    "now",
    "no",
    "loop",
    "me"
};
const size_t KEYWORDS_SIZES[] = {
    3,
    4,
    3,
    5,
    3
};


const int NUM_OPERATORS = 13;

const char* OPERATORS_ARR[] = {
    "->",// assignment and return
    "~->",// typecast
    "<-",// func and method call
    "~<-",// class constructor 
    "+",// add
    "~+",// subtract
    "*",// mult
    "~*",// div
    "=",// is equal to
    "~=",// not equal
    ">",// greater than
    "<",// less than
    "."// index and class var/method getting
};

const size_t OPERATORS_SIZES[] = {
    3,
    4,
    3,
    4,
    2,
    3,
    2,
    3,
    2,
    3,
    2,
    2,
    2
};