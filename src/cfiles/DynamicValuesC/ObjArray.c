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

        case COMP_FUNC_OBJ:
            ret = myMalloc(sizeof(CompFuncObj));
            assert(ret && "malloc a func obj failed while copying obj");
            memcpy(ret, this_obj, sizeof(CompFuncObj));
            return ret;
        case USER_FUNC_OBJ:
            ret = myMalloc(sizeof(UserFuncObj));
            assert(ret);
            memcpy(ret, this_obj, sizeof(UserFuncObj));
            return ret;
        case CLASS_OBJ:
            ret = myMalloc(sizeof(ClassObj));
            assert(ret);
            ((ClassObj*)ret)->type = CLASS_OBJ;
            ((ClassObj*)ret)->constructor = copyObj( ((ClassObj*)this_obj)->constructor );
            return ret;
        case INSTANCE_OBJ:
            ret = myMalloc(sizeof(InstanceObj));
            assert(ret);
            ((InstanceObj*)ret)->type = INSTANCE_OBJ;
            ((InstanceObj*)ret)->vars_methods = deepCopyStorage( ((InstanceObj*)this_obj)->vars_methods );
            ((InstanceObj*)ret)->_constructor_tell = ((InstanceObj*)this_obj)->_constructor_tell;
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
        case CLASS_OBJ:
            myFree(((ClassObj*)this_obj)->constructor);
            myFree(this_obj);
            break;
        case INSTANCE_OBJ:            
            freeStorageContents(&(((InstanceObj*)this_obj)->vars_methods));
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
            case COMP_FUNC_OBJ:
            case USER_FUNC_OBJ:
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
    case COMP_FUNC_OBJ: 
        dst = myMalloc(64); assert(dst);
        sprintf(dst, "CompFuncObj(Argc = %d)", ((CompFuncObj*)obj)->num_args);
        break;
    case USER_FUNC_OBJ:
        dst = myMalloc(64); assert(dst);
        sprintf(dst, "UserFuncObj(Argc = %d)", ((UserFuncObj*)obj)->num_args);
        break;
    case CLASS_OBJ:
        dst = myMalloc(128); assert(dst);
        sprintf(dst, "ClassObj( Argc = %d, line = %d )", 
            ((ClassObj*)obj)->constructor->num_args, ((ClassObj*)obj)->constructor->line_number);
        break;
    case INSTANCE_OBJ: {
        dst = myMalloc(64); assert(dst);
        size_t dst_size = 64;

        size_t l = 0;

        sprintf(dst, "InstanceObj( ");
        l = strlen(dst);

        InstanceObj* i_obj = obj;
        for (uint i = 0; i < i_obj->vars_methods.length; i++){
            if (i_obj->vars_methods.identifiers[i].union_mode == NNPSTR_UNIONMODE_BUFFER){
                l += strlen(i_obj->vars_methods.identifiers[i].string.buffer);
                if (l + 1 >= dst_size) {
                    dst_size = 2 * l + 1;
                    dst = realloc(dst, dst_size); 
                    assert(dst);
                }
                strcat(dst, i_obj->vars_methods.identifiers[i].string.buffer);
            } else {
                l += strlen(i_obj->vars_methods.identifiers[i].string.dyn_str);
                if (l + 1 >= dst_size) {
                    dst_size = 2 * l + 1;
                    dst = realloc(dst, dst_size); 
                    assert(dst);
                }
                strcat(dst, i_obj->vars_methods.identifiers[i].string.dyn_str);
            }
            l += 3;
            if (l + 3 >= dst_size) {
                dst_size = 2 * l + 1;
                dst = realloc(dst, dst_size); 
                assert(dst);
            }
            strcat(dst, " = "); 

            char* temp = objValtoDynAllocStr(i_obj->vars_methods.objs[i]);
            l += strlen(temp);
            if (l + 1 >= dst_size) {
                dst_size = 2 * l + 1;
                dst = realloc(dst, dst_size); 
                assert(dst);
            }
            strcat(dst, temp);
            myFree(temp);

            if (i + 1 < i_obj->vars_methods.length){
                l += 3;
                if (l + 3 >= dst_size) {
                    dst_size = 2 * l + 1;
                    dst = realloc(dst, dst_size); 
                    assert(dst);
                }
                strcat(dst, ", ");
            }
        }

        l += 2;
        if (l + 2 >= dst_size) {
            dst_size = 2 * l + 1;
            dst = realloc(dst, dst_size); 
            assert(dst);
        }
        strcat(dst, " )");

        break;
    }
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
    case COMP_FUNC_OBJ: {
        enum PrebuiltFuncs_e function_type_1 = ((CompFuncObj*)obj1)->func_type;
        return (function_type_1 == ((CompFuncObj*)obj2)->func_type);
        // if (function_type_1 != ((CompFuncObj*)obj2)->func_type)
        //     return false;
        }
    case USER_FUNC_OBJ:
        return ((UserFuncObj*)obj1)->declaration_tell   ==   ((UserFuncObj*)obj2)->declaration_tell;

    case CLASS_OBJ:
        if (*(Datatype_e*)obj2 != CLASS_OBJ) return false;
        return objsEqual(((ClassObj*)obj1)->constructor, ((ClassObj*)obj2)->constructor);
    case INSTANCE_OBJ:
        printf("INSTANCES not implemented yet!\n");
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