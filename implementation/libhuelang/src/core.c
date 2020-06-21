#if INTERFACE

typedef struct Environment Environment;

#define WORDWORD "word"
#include "environment.h"
#include "type.h"
#endif

#include "core.h"

void __primary(Environment *env) {
  if (env->execution_stack == NULL) return;
  Word word = Environment_PopExecution(env);
  // Obtain the definition of the word.
  Definition worddef = DefinitionTable_GetDefinition(env->definition_table, word);
  // Obtain the type of the definition
  Word primarydefinitiontype = DefinitionTable_TokToWord(env->definition_table, PRIMARYDEFINITIONWORD);
  Word definitiontype = worddef.type;
  if ((primarydefinitiontype.major == definitiontype.major) & 
    (primarydefinitiontype.minor == definitiontype.minor)) {
    void (*defhandler)(Environment*) = worddef.value.pointer;
    defhandler(env);
    return;
  }
  Definition definitiontypede = DefinitionTable_GetDefinition(env->definition_table, definitiontype);
  // Verify if the type definition is primary
  if ((definitiontypede.type.major == primarydefinitiontype.major) & 
    (definitiontypede.type.minor == primarydefinitiontype.minor)) {
    // Call the primary definition handler.
    void (*defhandler)(Environment*) = definitiontypede.value.pointer;
    defhandler(env);
  }
}

void Core_Initialize(Environment *env) {
  // This is the best piece of code I've written in my life.
  Word wordword = DefinitionTable_TokToWord(env->definition_table, WORDWORD);
  Types_RegisterAtomicType(env, wordword, sizeof(Word));

  Word primaryword = DefinitionTable_TokToWord(env->definition_table, PRIMARYDEFINITIONWORD);
  Definition primarydefinition;
  primarydefinition.type = primaryword;
  primarydefinition.value.pointer = &__primary;
  DefinitionTable_SetDefinition(env->definition_table, primaryword, primarydefinition);
}