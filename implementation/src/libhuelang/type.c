#if INTERFACE
typedef struct Environment Environment;
#include "environment.h"
#define ATOMTYPEDEFWORD "atomtypedef"
#endif

#include "type.h"

void Types_RegisterAtomicType(Environment *env, Word typename, unsigned int size) {
    Word atomtypedefword = DefinitionTable_TokToWord(env->definition_table, ATOMTYPEDEFWORD);
    Definition typedefinition;
    typedefinition.type = atomtypedefword;
    typedefinition.value.number = size;
    DefinitionTable_SetDefinition(env->definition_table, typename, typedefinition);
}