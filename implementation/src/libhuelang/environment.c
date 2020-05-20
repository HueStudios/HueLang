#if INTERFACE

#include <stdlib.h>
#include "definition.h"
#include "type.h"

#define PREPROCESSWORD "!preprocess"

typedef struct ExecutionStackNode {
  Word value;
  ExecutionStackNode *next;
} ExecutionStackNode; 

typedef struct Environment {
  DefinitionTable *definition_table;
  ExecutionStackNode *execution_stack;
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

// Pop a word from the execution stack
Word Environment_PopExecution(Environment *self) {
  ExecutionStackNode *old = self->execution_stack;
  Word result = old->value;
  self->execution_stack = old->next;
  return result;
}

// Forcibly evaluate a word in this environment.
void Environment_ForciblyEvaluate(Environment *self, Word word) {
  // TODO
} 

// Enter the execution loop.
void Environment_Run(Environment *self) {
  while (self->execution_stack != NULL) {
    Word preprocessword = DefinitionTable_TokToWord(self->definition_table, PREPROCESSWORD);
    Environment_ForciblyEvaluate(self, preprocessword);
  }
}

// Man, this is becoming messier than I originally anticipated XD