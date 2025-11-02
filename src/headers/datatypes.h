
#ifndef DATATYPES_H
#define DATATYPES_H

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>






#include "ObjectsH/DatatypeEnum.h"
#include "ObjectsH/ObjTypedefsMethodStructs.h"



#include "LoggingH/LogMessage.h"

// ALL OBJECTS STRUCTURE:
// 1. datatype enum
// 2. operators struct pointer
// 3. methods struct pointer
// 4. value

// This order because the first three members of the struct are all constant size
// but the last is object dependent

#include "ObjectsH/NumObj.h"
#include "ObjectsH/BoolObj.h"
#include "ObjectsH/DatatypeObj.h"
#include "ObjectsH/ListObj.h"
#include "ObjectsH/StrObj.h"

#include "ObjectsH/FuncObj.h"




#include "DynamicValuesH/Storage.h"
#include "DynamicValuesH/NnpStr.h"
#include "DynamicValuesH/ObjArray.h"





#include "FileParsingH/KeywordsAndOperators.h"
#include "FileParsingH/FileReading.h"
#include "FileParsingH/TokenInerpertation.h"




#include "FileExecutionH/Simplification.h"
#include "FileExecutionH/StandardMethodsFunctions.h"




// struct _FuncObj_t {
//     Datatype_e type;
//     fpos_t declaration_loc;
//     uint16_t num_args;
//     ClassObj* class_loc;

//     OperatorsStruct* operators;
// };


// struct _ClassObj_t {
//     Datatype_e type;
//     NnpStr* instance_vars;
//     uint num_instances_vars;

//     FuncObj* constructor;
//     FuncObj* methods;

//     OperatorsStruct* operators;
// };


// struct _InstanceObj_t {
//     Datatype_e type;
//     ClassObj* inherits_from;
//     ObjArray values;
//     uint function_level;
// };





#endif