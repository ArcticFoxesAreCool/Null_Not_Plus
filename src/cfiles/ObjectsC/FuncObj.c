#include "../../headers/datatypes.h"

// TRUNCATE, STR_LEN, SUBSTRING, POP, FIND, INSERT, SET,
//     SPEAK, LISTEN


#define NUM_PREBUILTS_IDENTIFIERS 10

const char* prebuilts_identifiers[] = {
    "truncate!",
    "strLen!",
    "substring!",
    "pop!",
    "find!",
    "insert!",
    "set!",
    "speak!",
    "asciiToCh!",
    "listen!"
};



static void loadHelper(enum PrebuiltFuncs_e prebuilt, int identifier_index, Storage* p_store, int num_ar){
    FuncObj tmp = {.type = FUNC_OBJ, .func_type = prebuilt, .num_args = num_ar};
    NnpStr tmp_str = makeNnpStr(prebuilts_identifiers[identifier_index]);
    appendStorage(p_store, &tmp, &tmp_str);
    freeNnpStr(&tmp_str);

}



void loadPrebuiltsIntoStorage(Storage* p_store){
    loadHelper(TRUNCATE_M, 0, p_store, 0);
    loadHelper(STR_LEN_M, 1, p_store, 0);
    loadHelper(SUBSTRING_M, 2, p_store, 2);
    loadHelper(POP_M, 3, p_store, 1);
    loadHelper(FIND_M, 4, p_store, 1);
    loadHelper(INSERT_M, 5, p_store, 2);
    loadHelper(SET_M, 6, p_store, 2);
    loadHelper(SPEAK_F, 7, p_store, 1);
    loadHelper(ASCII_TO_CH_F, 8, p_store, 1);
    loadHelper(LISTEN_F, 9, p_store, 0);
}

