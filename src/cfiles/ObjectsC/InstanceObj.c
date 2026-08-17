#include "../../headers/datatypes.h"

InstanceObj* constructInstanceObj(ClassObj* class){
    logFuncStart(FUNCTION_CALLS, "start con InstanceObj: tell = %d, num parameters = %d\n", class->constructor->declaration_tell, class->constructor->num_args);

    InstanceObj* ret = myMalloc(sizeof(InstanceObj));
    assert(ret);
    ret->type = INSTANCE_OBJ;
    ret->vars_methods = (Storage) {
        .capacity = 8,
        .identifiers = myMalloc(sizeof(NnpStr) * 8),
        .objs = myMalloc(sizeof(object_p) * 8),
        .length = 0
    };

    logFuncEnds(FUNCTION_CALLS, "finish con InstanceObj: tell = %ld\n", ret->_constructor_tell);
    return ret;
}