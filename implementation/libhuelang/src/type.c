#if INTERFACE
typedef struct Environment Environment;
#include "environment.h"
#define ATOMTYPEDEFWORD "atomic"
#define COMPOUNDTYPEDEFWORD "compound"
#endif

#include "type.h"

void Types_RegisterAtomicType(Environment *env, Word typename,
    unsigned int size) {
  Word atomtypedefword = DefinitionTable_TokToWord(env->definition_table,
    ATOMTYPEDEFWORD);
  Definition typedefinition;
  typedefinition.type = atomtypedefword;
  typedefinition.value.number = size;
  DefinitionTable_SetDefinition(env->definition_table, typename,
    typedefinition);
}

void Types_RegisterCompoundType(Environment *env, Word typename,
  unsigned long (*type_handler)(Environment*, Word*)) {
  Word compoundtypedefword = DefinitionTable_TokToWord(env->definition_table,
    COMPOUNDTYPEDEFWORD);
  Definition typedefinition;
  typedefinition.type = compoundtypedefword;
  typedefinition.value.pointer = type_handler;
  DefinitionTable_SetDefinition(env->definition_table, typename,
    typedefinition);
}

unsigned long Types_ResolveTypeSize(Environment *env, Word *type) {
  Definition typedefinition =
    DefinitionTable_GetDefinition(env->definition_table, *type);
  Word typetype = typedefinition.type;
  Word atomtypedefword = DefinitionTable_TokToWord(env->definition_table,
    ATOMTYPEDEFWORD);
  Word cptypedefword = DefinitionTable_TokToWord(env->definition_table,
    COMPOUNDTYPEDEFWORD);
  if ((typetype.major == atomtypedefword.major) &
    (typetype.minor == atomtypedefword.minor)) {
    return typedefinition.value.number;
  } else if ((typetype.major == cptypedefword.major) &
    (typetype.minor == cptypedefword.minor)) {
    unsigned long (*type_handler)(Environment*,Word*)
      = typedefinition.value.pointer;
    return type_handler(env,type);
  }
  return 0;
}
