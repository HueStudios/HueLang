#if INTERFACE

#include "environment.h"

#define SECONDARYDEFINITIONWORD "secondary"

typedef WordArrayList SecondaryDefinition;

#endif

#include "secondary.h"

void __secondary(Environment *env) {
  if (env->execution_stack == NULL) {
    return;
  }
  Word word = Environment_PopExecution(env);
  // Obtain the definition of the word.
  Definition worddef = DefinitionTable_GetDefinition(env->definition_table,
    word);
  SecondaryDefinition *wordlist = worddef.value.pointer;
  for (unsigned int i = 0; i < wordlist->size; i++) {
    Environment_PushExecution(env,wordlist->data[i]);
  }
}

void Secondary_Initialize(Environment *env) {
  Word secondaryword = DefinitionTable_TokToWord(env->definition_table,
    SECONDARYDEFINITIONWORD);
  Environment_AddPrimaryDefinition(env, secondaryword, &__secondary);
}
