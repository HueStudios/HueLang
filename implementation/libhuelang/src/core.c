#if INTERFACE

typedef struct Environment Environment;

#define WORDWORD "word"
#include <stdio.h>
#include "environment.h"
#include "type.h"
#endif

#include "core.h"

void __primary(Environment *env) {
  if (env->execution_stack == NULL) {
    printf("WARNING - Called definition handler on an empty exec stack?\n");
    return;
  }
  Word word = Environment_PopExecution(env);
  // Obtain the definition of the word.
  Definition worddef = DefinitionTable_GetDefinition(env->definition_table, word);
  void (*defhandler)(Environment*) = worddef.value.pointer;
  defhandler(env);
}

void __undefined(Environment *env) {
  if (env->execution_stack == NULL) {
    printf("WARNING - Called definition handler on an empty exec stack?\n");
    return;
  }
  Word word = Environment_PopExecution(env);
  char *name = DefinitionTable_GetName(env->definition_table, word);
  printf("Undefined word %s\n", name);
}

void Core_Initialize(Environment *env) {
  // This is the best piece of code I've written in my life.
  Word wordword = DefinitionTable_TokToWord(env->definition_table, WORDWORD);
  Types_RegisterAtomicType(env, wordword, sizeof(Word));

  Word primaryword = DefinitionTable_TokToWord(env->definition_table, PRIMARYDEFINITIONWORD);
  Environment_AddPrimaryDefinition(env, primaryword, &__primary);

  Word undefinedword = DefinitionTable_TokToWord(env->definition_table, UNDEFINEDWORD);
  Environment_AddPrimaryDefinition(env, undefinedword, &__undefined);
}