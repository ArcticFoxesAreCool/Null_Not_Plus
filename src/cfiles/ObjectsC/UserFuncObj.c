#include "../../headers/datatypes.h"

UserFuncObj* constructUserFuncObj(long tell, int num_parameters, int line_number){
    logFuncStart(FUNCTION_CALLS, "start con UserFuncObj: tell = %ld, num parameters = %d\n", tell, num_parameters);

    UserFuncObj* ret = myMalloc(sizeof(UserFuncObj));
    assert(ret);
    ret->declaration_tell = tell;
    ret->num_args = num_parameters;
    ret->line_number = line_number;
    ret->type = USER_FUNC_OBJ;

    logFuncEnds(FUNCTION_CALLS, "finish con UserFuncObj: tell = %ld, num parameters = %d\n", ret->declaration_tell, ret->num_args);
    return ret;
}