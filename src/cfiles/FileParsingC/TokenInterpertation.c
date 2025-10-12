
#include "../../headers/datatypes.h"


TokenTyper tok_types = {
    .size = 0,
    .types = NULL
};


void initTok_types(int initial_size){
    assert(tok_types.size == 0 && tok_types.types == NULL && initial_size > 0);
    tok_types.size = initial_size;
    tok_types.types = malloc(sizeof(enum Token_Types_e) * initial_size);
    assert(tok_types.types);
}


void freeTok_types(){
    assert(tok_types.size != 0 && tok_types.size);
    free(tok_types.types);
    tok_types.types = NULL;

    tok_types.size = 0;
}


static bool isKeyword(const char* token);
static bool isOperator(const char* token);
static Datatype_e findValueType(const char* token);


void getTok_types(){

    extern Reader nian;
    assert(nian.charv && nian.token_indexes && nian.sz > 1 && nian.tok_ind_capacity > 0);

    if (nian.tok_ind_len == 0) return;

    if (nian.tok_ind_len > tok_types.size){
        assert(tok_types.size > 0);
        while (nian.tok_ind_len > tok_types.size){
            tok_types.size *= 2;
        }
        tok_types.types = realloc(tok_types.types, tok_types.size * sizeof(enum Token_Types_e));
        assert(tok_types.types);
    }

    for (int i = 0; i < nian.tok_ind_len; i++){
        if (isKeyword( nian.charv + nian.token_indexes[i] )){
            tok_types.types[i] = KEYWORD;
        } else if (isOperator( nian.charv + nian.token_indexes[i] )){
            tok_types.types[i] = OPERATOR;
        } else if (findValueType( nian.charv + nian.token_indexes[i] ) != NAO){
            tok_types.types[i] = VALUE;
        } else {
            tok_types.types[i] = VARIABLE;
        }
    }
}













static bool isKeyword(const char* token){
    for(int i = 0; i < NUM_KEYWORDS; i++){
        if (strncmp(token, KEYWORDS_ARR[i], KEYWORDS_SIZES[i]) == 0){
            return true;
        }
    }
    return false;
}

static bool isOperator(const char* token){
    for(int i = 0; i < NUM_OPERATORS; i++){
        if (strncmp(token, OPERATORS_ARR[i], OPERATORS_SIZES[i]) == 0){
            return true;
        }
    }
    return false;
}


static bool isDattype(const char* token);


static Datatype_e findValueType(const char* token){
    // printf("\ttok: %s\n", token);
    if ( fabs( strtod(token, NULL) ) > 2 * __DBL_EPSILON__ || token[0] == '0'){
        return NUM_OBJ;
    } else if (strncmp(token, "True", 5) == 0 || strncmp(token, "False", 6) == 0){
        return BOOL_OBJ;
    } else if (isDattype(token)){
        return DATATYPE_OBJ;
    } else if (token[0] == '\"'){
        return STR_OBJ;
    } else if (strncmp("|", token, 2) == 0){
        return LIST_OBJ;
    }

    return NAO;
}




static bool isDattype(const char* token){
    if (strncmp("BoolObj", token, 8) == 0){
        return true;
    } else if (strncmp("DatatypeObj", token, 12) == 0){
        return true;
    } else if (strncmp("ListObj", token, 8) == 0){
        return true;
    } else if (strncmp("NumObj", token, 7) == 0){
        return true;
    } else if (strncmp("StrObj", token, 7) == 0){
        return true;
    }

    return false;
}