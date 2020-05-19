#if INTERFACE

#include "definition.h"
#include "type.h"

typedef struct Environment {
  DefinitionTable *definition_table;
} Environment;

#endif

#include "environment.h"


// Create a runtime environment and return a pointer to it.
Environment *Environment_Create() {
  Environment *self = malloc(sizeof(Environment));
  self->definition_table = DefinitionTable_Create();

  // This is the best piece of code I've written in my life.
  Word wordword = DefinitionTable_TokToWord(self->definition_table, "word");
  Types_RegisterAtomicType(self, wordword, sizeof(Word));


  return self;
}