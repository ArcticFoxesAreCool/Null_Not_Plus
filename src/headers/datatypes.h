
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
#include <float.h>





#include "ObjectsH/DatatypeEnum.h"
#include "ObjectsH/ObjTypedefsMethodStructs.h"



#include "LoggingH/LogMessage.h"
#include "LoggingH/MemoryAllocationTracking.h"

// ALL VALUE OBJECTS STRUCTURE:
// 1. datatype enum
// 2. operators struct pointer
// 3. methods struct pointer
// 4. value

// This order because the first three members of the struct are all constant size
// but the last is object dependent

// Value Objects
#include "ObjectsH/NumObj.h"
#include "ObjectsH/BoolObj.h"
#include "ObjectsH/DatatypeObj.h"
#include "ObjectsH/ListObj.h"
#include "ObjectsH/StrObj.h"

// Commander Objects
// ALL COMMANDER OBJECT STRUCTURE:
// 1. datatype enum
// 2. custom
#include "ObjectsH/CompFuncObj.h"
#include "ObjectsH/UserFuncObj.h"
#include "ObjectsH/ClassObj.h"
#include "ObjectsH/InstanceObj.h"


#include "DynamicValuesH/Storage.h"
#include "DynamicValuesH/NnpStr.h"
#include "DynamicValuesH/ObjArray.h"





#include "FileParsingH/KeywordsAndOperators.h"
#include "FileParsingH/FileReading.h"
#include "FileParsingH/TokenInerpertation.h"




#include "FileExecutionH/Simplification.h"
#include "FileExecutionH/StandardMethodsFunctions.h"
#include "FileExecutionH/VariableAssignment.h"
#include "FileExecutionH/ExecutionManager.h"
#include "FileExecutionH/UserFunctionExecution.h"
#include "FileExecutionH/BlockTracking.h"
#include "FileExecutionH/ClassHandling.h"






#endif