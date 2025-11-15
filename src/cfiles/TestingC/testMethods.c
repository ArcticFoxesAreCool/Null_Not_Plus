
#include "../../headers/TestingH/testMethods.h"




void testNumMethods(){
    puts("TESTING NUM OBJS");

    NumObj* nums[] = {
        constructNumObj(-5.25),
        constructNumObj(-1.6),
        constructNumObj(0.3),
        constructNumObj(1),
        constructNumObj(5.8)
    };


    for(uint i = 0; i < 5; i++){
        nums[i]->methods->truncate(nums[i]);
        printf("%lf\n", nums[i]->value);
    }


    for(uint i = 0; i < 5; i++){
        freeObj(nums[i]);
    }

}

void testStrMethods(){
    puts("TESTING STRLEN");

    StrObj* strs[] = {
        constructStrObj_char(""),
        constructStrObj_char("abcdefg"),
        constructStrObj_char("aQuite long"),
        constructStrObj_char("so very really long str"),
        constructStrObj_char(""),
        constructStrObj_char("a"),
        constructStrObj_char("ABCde")
    };


    NumObj* lens[sizeof(strs) / sizeof(StrObj*)];

    puts("STRLEN!!");
    for(uint i = 0; i < sizeof(strs) / sizeof(StrObj*); i++){
        lens[i] = strs[i]->methods->str_len(strs[i]);

        if (strs[i]->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
            printf("%24s: %lf\n",strs[i]->value.string.buffer, lens[i]->value);
        } else {
            printf("%24s: %lf\n",strs[i]->value.string.dyn_str, lens[i]->value);
        }

        freeObj(lens[i]);
        lens[i] = NULL;
    }


    NumObj* start = constructNumObj(16);
    NumObj* end = constructNumObj(22);
    puts("\n\n");

    StrObj* substr;
    for(uint i = 0; i < sizeof(strs) / sizeof(StrObj*); i++){
        // printf("%d\n", i);
        if (i != 3) continue;

        substr = strs[i]->methods->substring(strs[i], start, end);

        if (strs[i]->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
            printf("strs[%d]: %24s: [%.0lf, %.0lf] -> %s\n", i, strs[i]->value.string.buffer, start->value, end->value, substr->value.string.buffer);
        } else if (substr->value.union_mode == NNPSTR_UNIONMODE_BUFFER){
            printf("strs[%d]: %24s: [%.0lf, %.0lf] -> %s\n", i, strs[i]->value.string.dyn_str, start->value, end->value, substr->value.string.buffer);
        } else {
            printf("strs[%d]: %24s: [%.0lf, %.0lf] -> %s\n", i, strs[i]->value.string.dyn_str, start->value, end->value, substr->value.string.dyn_str);
        }

        freeObj(substr);
    }


    for(uint i = 0; i < sizeof(strs) / sizeof(StrObj*); i++){
        freeObj(strs[i]);
    }
    putchar('\n');
}





void testListMethods(){

    ObjArray temp = {
        .capacity = 32,
        .length = 22,
        .objs = myMalloc(32 * sizeof(object_p))
    };
    assert(temp.objs);
    constructObjsTest(temp.objs);


    
    
    ListObj* lst = constructListObj(&temp);
    for(uint i = 0; i < temp.length; i++){
        myFree(temp.objs[i]);
    }
    myFree(temp.objs);
    

    char* st;

    object_p a[] = {
        constructBoolObj(false),
        constructBoolObj(true),
        constructNumObj(100),
        constructDatatypeObj(CLASS_OBJ),
        constructNumObj(0.0),
        constructNumObj(-0.5),
        constructStrObj_char("short"),
        constructStrObj_char("wow super long string"),
        copyObj(lst->values.objs[21])
    };

    
    for(unsigned long i = 0; i < sizeof(a) / sizeof(object_p); i++){
        NumObj* l = lst->methods->find(lst, a[i]);

        st = objValtoDynAllocStr(a[i]);
        printf("\n[%.4lf]:: %s", l->value, st);
        myFree(st);

        freeObj(l);
        freeObj(a[i]);
    }

    putchar('\n');
    putchar('\n');

    BoolObj* g = constructBoolObj(true);
    NumObj* ga = constructNumObj(4);
    lst->methods->set(lst, g, ga);



    NumObj* y = constructNumObj(22);
    lst->methods->insert(lst, y, y);
    // lst->methods->insert(lst, y, y);


    for(uint i = 0; i < lst->values.length; i++){
        st = objValtoDynAllocStr(lst->values.objs[i]);
        printf("list[%d] = %s\n", i, st);
        myFree(st);
    }


    freeObj(y);

    freeObj(g);
    freeObj(ga);
    freeObj(lst);


}



