#if INTERFACE
typedef struct Environment Environment;
#include "environment.h"
#define ATOMTYPEDEFWORD "atomic"
#define COMPOUNDTYPEDEFWORD "compound"
#endif

#include "type.h"

void Types_RegisterAtomicType(Environment *env, Word typename, unsigned int size) {
  Word atomtypedefword = DefinitionTable_TokToWord(env->definition_table, ATOMTYPEDEFWORD);
  Definition typedefinition;
  typedefinition.type = atomtypedefword;
  typedefinition.value.number = size;
  DefinitionTable_SetDefinition(env->definition_table, typename, typedefinition);
}

void Types_RegisterCompoundType(Environment *env, Word typename, unsigned int (*type_handler)(Word*)) {
  Word compoundtypedefword = DefinitionTable_TokToWord(env->definition_table, COMPOUNDTYPEDEFWORD);
  Definition typedefinition;
  typedefinition.type = compoundtypedefword;
  typedefinition.value.pointer = type_handler;
  DefinitionTable_SetDefinition(env->definition_table, typename, typedefinition);
}