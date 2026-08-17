#include "../../headers/datatypes.h"


ClassObj* constructClassObj(long tell, int num_parameters, int line_number){

    logFuncStart(FUNCTION_CALLS, "start con ClassObj: tell = %ld, num parameters = %d\n", tell, num_parameters);

    ClassObj* ret = myMalloc(sizeof(ClassObj));
    assert(ret);
    ret->type = CLASS_OBJ;
    ret->constructor = constructUserFuncObj(tell, num_parameters, line_number);

    logFuncEnds(FUNCTION_CALLS, "finish con ClassObj: tell = %ld, num parameters = %d\n", ret->constructor->declaration_tell, ret->constructor->num_args);
    return ret;

}
