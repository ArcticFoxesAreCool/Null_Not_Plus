#include "../../headers/datatypes.h"

extern OperatorsStruct listOperators;


object_p copyObj(const object_p this_obj){
    assert(this_obj && "copying obj, is null");
    Datatype_e dat = *((Datatype_e*)this_obj);
    
    object_p ret;

    switch(dat){
        case NUM_OBJ:
            ret = myMalloc(sizeof(NumObj));
            assert(ret && "malloc a num obj failed while copying obj");
            memcpy(ret, this_obj, sizeof(NumObj));
            return ret;
        case BOOL_OBJ:
            ret = myMalloc(sizeof(BoolObj));
            assert(ret && "malloc a bool obj failed while copying obj");
            memcpy(ret, this_obj, sizeof(BoolObj));
            return ret;
        case DATATYPE_OBJ:
            ret = myMalloc(sizeof(DatatypeObj));
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
            ret = myMalloc(sizeof(FuncObj));
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

            

            if (  ((StrObj*)this_obj)->value.union_mode == NNPSTR_UNIONMODE_POINTER  ){
                myFree( ((StrObj*)this_obj)->value.string.dyn_str );
                // printf("  Unionmode Pointer");
            }
            // putchar('\n');


            myFree(this_obj);
            break;
        case LIST_OBJ:
            // printf("A\n");fflush(stdout);
            
            for(uint i = 0; i < ((ListObj*)this_obj)->values.length; i++){
              
                freeObj(  ((ListObj*)this_obj)->values.objs[i]  );
            }
            // putchar('\n');
            // printf("B\n");fflush(stdout);

            myFree(((ListObj*)this_obj)->values.objs);
            myFree(this_obj);
            break;

        default:
          

            myFree(this_obj);
    }
}






char* objValtoDynAllocStr(object_p obj){
    char* dst = NULL;

    Datatype_e dat = *((Datatype_e*)obj);
    switch (dat){
    case BOOL_OBJ:
        dst = myMalloc(6); assert(dst);
        if (((BoolObj*)obj)->value != 0){
            sprintf(dst, "True");
        } else {
            sprintf(dst, "False");
        }
        break;
    case DATATYPE_OBJ: {

            dst = myMalloc(16); assert(dst);
            Datatype_e dat_val = ((DatatypeObj*)obj)->value;
            switch (dat_val){
            case BOOL_OBJ:
                sprintf(dst, "BoolObj");
                break;
            case DATATYPE_OBJ:
                sprintf(dst, "DatatypeObj");
                break;
            case LIST_OBJ:
                sprintf(dst, "ListObj");
                break;
            case NUM_OBJ:
                sprintf(dst, "NumObj");
                break;
            case STR_OBJ:
                sprintf(dst, "StrObj");
                break;
            case FUNC_OBJ:
                sprintf(dst, "FuncObj");
                break;
            case CLASS_OBJ:
                sprintf(dst, "ClassObj");
                break;
            case INSTANCE_OBJ:
                sprintf(dst, "InstanceObj");
                break;
            default:
                puts("objValtoStr unkown value");
                exit(1);
            }
        }
        break;
    case NUM_OBJ:
        dst = myMalloc(32); assert(dst);    
        snprintf(dst, 32, "%lf", ((NumObj*)obj)->value);
        break;
    case STR_OBJ:
        if (((StrObj*)obj)->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
            dst = myMalloc(STRING_U_BUFFER_SIZE + 2); assert(dst);
            sprintf(dst, "\"%s\"", ((StrObj*)obj)->value.string.buffer);
        } else {
            dst = myMalloc(strlen(((StrObj*)obj)->value.string.dyn_str) + 3); assert(dst);
            sprintf(dst, "\"%s\"", ((StrObj*)obj)->value.string.dyn_str);
        }
        break;
    case LIST_OBJ: {
        uint capacity = 64;
        size_t dst_len = 3;

        char* tmp_val;
        size_t tmp_len;

        dst = myMalloc(capacity); assert(dst);
        sprintf(dst, "[ ");
        for(uint i = 0; i < ((ListObj*)obj)->values.length; i++){
            tmp_val = objValtoDynAllocStr(((ListObj*)obj)->values.objs[i]);
            tmp_len = strlen(tmp_val);

            if (tmp_len + dst_len + 3 > capacity){
                do { capacity *= 2; } while (tmp_len + dst_len + 3 > capacity);
                dst = myRealloc(dst, capacity); assert(dst);
            }
            strcat(dst, tmp_val);
            myFree(tmp_val);

            strcat(dst, " ");
            dst_len += tmp_len + 1;
        }
        strcat(dst, "]");
        }
        break;
    case FUNC_OBJ: 
        dst = myMalloc(32); assert(dst);
        sprintf(dst, "FuncObj(Argc = %d)", ((FuncObj*)obj)->num_args);
        break;
    default:
        // printf("dat: %d\n", dat);
        puts("ObjValtoStr obj datatype not found");
        exit(1);
    }

    return dst;
}









void appendInObjArray(ObjArray* p_obj_arr, const object_p this_obj){
    if (p_obj_arr->capacity == p_obj_arr->length){
        p_obj_arr->capacity *= 2;
        p_obj_arr->objs = myRealloc( p_obj_arr->objs, sizeof(object_p) * p_obj_arr->capacity);
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
    // myFree(p_obj_arr->objs);
}


void insertInObjArray(ObjArray* p_obj_arr, const object_p this_obj, uint index){
    assert(p_obj_arr && this_obj && "insert into obj arr");
    if (p_obj_arr->length == p_obj_arr->capacity){
        p_obj_arr->capacity *= 2;
        p_obj_arr->objs = myRealloc(p_obj_arr->objs, sizeof(object_p) * p_obj_arr->capacity );
        assert(p_obj_arr->objs && "realloc obj arr insert");
    }
    if (index > p_obj_arr->length){ puts("insert out of bounds"); exit(1);}


    memmove(  &(p_obj_arr->objs[index + 1]), &(p_obj_arr->objs[index]), (sizeof(object_p) * (p_obj_arr->length - index))  );
    p_obj_arr->length += 1;
    p_obj_arr->objs[index] = copyObj(this_obj);
// //  0  1  2  3  4  5
// //  a  b  c  d
}



static bool twoListsEqual(const ListObj* lst1, const ListObj* lst2);



bool objsEqual(const object_p obj1, const object_p obj2){
    assert(obj1 && obj2);
    Datatype_e obj1_type = *(Datatype_e*)obj1;
    if (obj1_type != *(Datatype_e*)obj2) return false;

    switch (obj1_type){
    case BOOL_OBJ:
        return ((BoolObj*)obj1)->value == ((BoolObj*)obj2)->value;
    case DATATYPE_OBJ:
        return ((DatatypeObj*)obj1)->value == ((DatatypeObj*)obj2)->value;
    case NUM_OBJ: {
        double val1 = ((NumObj*)obj1)->value;
        double val2 = ((NumObj*)obj2)->value;
        return fabs(val1 - val2) < 2 * __DBL_EPSILON__;
    }
    case LIST_OBJ:
        return twoListsEqual(obj1, obj2);
    case STR_OBJ:
        return NnpStrEq(  &(((StrObj*)obj1)->value),  &(((StrObj*)obj2)->value) );
    case FUNC_OBJ: {
        enum PrebuiltFuncs_e function_type_1 = ((FuncObj*)obj1)->func_type;
        if (function_type_1 != ((FuncObj*)obj2)->func_type)
            return false;
        if (function_type_1 == USER_FUNC){
            return ((FuncObj*)obj1)->f_loc == ((FuncObj*)obj2)->f_loc;
        }
        return true;
    }

    case INSTANCE_OBJ:
    case CLASS_OBJ:
        printf("CLASSES not implemented yet!\n");
        exit(1);
    case NAO:
        printf("objsEqual should never be called on a NAO\n");
        exit(1);

    }
    printf("\n\nObjArray.c objsEqaul\tShould have returned!");
    exit(1);
}



static bool twoListsEqual(const ListObj* lst1, const ListObj* lst2){
    assert(lst1 && lst2);

    if (lst1->values.length != lst2->values.length) return false;

    for (uint i = 0; i < lst1->values.length; i++){
       
        if (objsEqual(lst1->values.objs[i], lst2->values.objs[i]) == false)
            return false;
    }

    return true;
}