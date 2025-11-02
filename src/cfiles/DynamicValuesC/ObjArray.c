#include "../../headers/datatypes.h"

extern OperatorsStruct listOperators;


object_p copyObj(const object_p this_obj){
    assert(this_obj && "copying obj, is null");
    Datatype_e dat = *((Datatype_e*)this_obj);
    
    object_p ret;

    switch(dat){
        case NUM_OBJ:
            ret = malloc(sizeof(NumObj));
            assert(ret && "malloc a num obj failed while copying obj");
            memcpy(ret, this_obj, sizeof(NumObj));
            return ret;
        case BOOL_OBJ:
            ret = malloc(sizeof(BoolObj));
            assert(ret && "malloc a bool obj failed while copying obj");
            memcpy(ret, this_obj, sizeof(BoolObj));
            return ret;
        case DATATYPE_OBJ:
            ret = malloc(sizeof(DatatypeObj));
            assert(ret && "malloc a datatype obj failed while copying obj");
            memcpy(ret, this_obj, sizeof(DatatypeObj));
            return ret;
        case LIST_OBJ:
            return (object_p)constructListObj(  &(((ListObj*)this_obj)->values)  );

        case STR_OBJ:
  
            if ( ((StrObj*)this_obj)->value.union_mode == NNPSTR_UNIONMODE_BUFFER ){
                return  (object_p)constructStrObj_char( ((StrObj*)this_obj)->value.string.buffer );
            } else {
                return  (object_p)constructStrObj_char( ((StrObj*)this_obj)->value.string.dyn_str );
            }

        case FUNC_OBJ:
            ret = malloc(sizeof(FuncObj));
            assert(ret && "malloc a func obj failed while copying obj");
            memcpy(ret, this_obj, sizeof(FuncObj));
            return ret;
        default:
            puts("copy object datatype not detected");
            exit(1);
            break;
    }
}





void freeObj(object_p this_obj){
    assert(this_obj && "freeing obj, is null");
    Datatype_e dat = *((Datatype_e*)this_obj);

    // char testing_printer[OBJ_PRINTING_CHAR_SIZE];

    switch(dat){
        case STR_OBJ:

            // objValtoStr(testing_printer, this_obj);
            // printf("String: %s", testing_printer);

            if (  ((StrObj*)this_obj)->value.union_mode == NNPSTR_UNIONMODE_POINTER  ){
                free( ((StrObj*)this_obj)->value.string.dyn_str );
                // printf("  Unionmode Pointer");
            }
            // putchar('\n');


            free(this_obj);
            break;
        case LIST_OBJ:
            // printf("A\n");fflush(stdout);
            
            for(uint i = 0; i < ((ListObj*)this_obj)->values.length; i++){
                // objValtoStr(testing_printer, ((ListObj*)this_obj)->values.objs[i] );
                // printf("List[%u]:\t%s\n\t", i, testing_printer);
                // printf("WHY %d\n", i);fflush(stdout);
                freeObj(  ((ListObj*)this_obj)->values.objs[i]  );
            }
            // putchar('\n');
            // printf("B\n");fflush(stdout);

            free(((ListObj*)this_obj)->values.objs);
            free(this_obj);
            break;

        default:
            // objValtoStr(testing_printer, this_obj);
            // printf("Default: %s\n", testing_printer);

            free(this_obj);
    }
}






void objValtoStr(char* dst, object_p obj){
    Datatype_e dat = *((Datatype_e*)obj);
    switch (dat){
    case BOOL_OBJ:
        if (((BoolObj*)obj)->value != 0){
            sprintf(dst, "true");
        } else {
            sprintf(dst, "false");
        }
        return;
    case DATATYPE_OBJ:
        {
            Datatype_e dat_val = ((DatatypeObj*)obj)->value;
            switch (dat_val){
            case BOOL_OBJ:
                sprintf(dst, "BoolObj");
                return;
            case DATATYPE_OBJ:
                sprintf(dst, "DatatypeObj");
                return;
            case LIST_OBJ:
                sprintf(dst, "ListObj");
                return;
            case NUM_OBJ:
                sprintf(dst, "NumObj");
                return;
            case STR_OBJ:
                sprintf(dst, "StrObj");
                return;
            case FUNC_OBJ:
                sprintf(dst, "FuncObj");
                return;
            case CLASS_OBJ:
                sprintf(dst, "ClassObj");
                return;
            case INSTANCE_OBJ:
                sprintf(dst, "InstanceObj");
                return;
            default:
                assert(false && "objValtoStr unkown value");
                return;
            }
        }
    case NUM_OBJ:
        sprintf(dst, "%lf", ((NumObj*)obj)->value);
        return;
    case STR_OBJ:
        if (((StrObj*)obj)->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
            sprintf(dst, "%s", ((StrObj*)obj)->value.string.buffer);
        } else {
            sprintf(dst, "%s", ((StrObj*)obj)->value.string.dyn_str);
        }
        return;
    case LIST_OBJ:
        sprintf(dst, "ListObj.len = %u", ((ListObj*)obj)->values.length);
        return;
    
    default:
        // printf("\n\n%d\n", dat);fflush(stdout);
        assert(false && "ObjValtoStr obj datatype not found");
    }
}









void appendInObjArray(ObjArray* p_obj_arr, const object_p this_obj){
    if (p_obj_arr->capacity == p_obj_arr->length){
        p_obj_arr->capacity *= 2;
        p_obj_arr->objs = realloc( p_obj_arr->objs, sizeof(object_p) * p_obj_arr->capacity);
        assert(p_obj_arr->objs && "grow obj arr fail");

    }

    p_obj_arr->objs[ p_obj_arr->length ] = copyObj(this_obj);
    p_obj_arr->length += 1;
}

void popInObjArray(ObjArray* p_obj_arr, uint index){
    if (index >= p_obj_arr->length){puts("pop obj index out of bounds"); exit(1);}

    freeObj(p_obj_arr->objs[index]);
    if (index + 1 != p_obj_arr->length){
        memmove(  &(p_obj_arr->objs[index]),  &(p_obj_arr->objs[index + 1]), sizeof(object_p) * (p_obj_arr->length - index - 1)  );
    }
    p_obj_arr->length--;
}

void freeObjArrayEntries(ObjArray* p_obj_arr){
    assert(p_obj_arr && "free obj arr when arr is null");
    for(uint i = 0; i < p_obj_arr->length; i++){
        freeObj(  p_obj_arr->objs[i]  );
        p_obj_arr->objs[i] = NULL;
    }
    // free(p_obj_arr->objs);
}


void insertInObjArray(ObjArray* p_obj_arr, const object_p this_obj, uint index){
    assert(p_obj_arr && this_obj && "insert into obj arr");
    if (p_obj_arr->length == p_obj_arr->capacity){
        p_obj_arr->capacity *= 2;
        p_obj_arr->objs = realloc(p_obj_arr->objs, sizeof(object_p) * p_obj_arr->capacity );
        assert(p_obj_arr->objs && "realloc obj arr insert");
    }
    if (index > p_obj_arr->length){ puts("insert out of bounds"); exit(1);}


    memmove(  &(p_obj_arr->objs[index + 1]), &(p_obj_arr->objs[index]), (sizeof(object_p) * (p_obj_arr->length - index))  );
    p_obj_arr->length += 1;
    p_obj_arr->objs[index] = copyObj(this_obj);
// //  0  1  2  3  4  5
// //  a  b  c  d
}
