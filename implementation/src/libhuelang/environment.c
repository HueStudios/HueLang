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

// Push a word to the execution stack
void Environment_PushExecution(Environment *self, Word topush) {
  ExecutionStackNode *newhead = malloc(sizeof(ExecutionStackNode));
  newhead->value = topush;
  newhead->next = self->execution_stack;
  self->execution_stack = newhead;
}

// Pop a word from the execution stack
Word Environment_PopExecution(Environment *self) {
  ExecutionStackNode *old = self->execution_stack;
  Word result = old->value;
  self->execution_stack = old->next;
  free(old);
  return result;
}

// Peek a word from the execution stack
Word Environment_PeekExecution(Environment *self) {
  return self->execution_stack->value;
}

// Forcibly evaluate a word in this environment.
void Environment_ForciblyEvaluate(Environment *self) {
  Word word = Environment_PeekExecution(self);
  // Obtain the definition of the word.
  Definition worddef = DefinitionTable_GetDefinition(self->definition_table, word);
  // Obtain the type of the definition
  Word definitiontype = worddef.type;
  Definition definitiontypede = DefinitionTable_GetDefinition(self->definition_table, definitiontype);
  // Verify if the type definition is primary
  Word primarydefinitiontype = DefinitionTable_TokToWord(self->definition_table, "primary");
  if ((definitiontypede.type.major == primarydefinitiontype.major) & 
    (definitiontypede.type.minor == primarydefinitiontype.minor)) {
    // Call the definition handler.
    void (*defhandler)(Environment*) = definitiontypede.value.pointer;
    defhandler(self);
  }
} 

// Enter the execution loop.
void Environment_Run(Environment *self) {
  while (self->execution_stack != NULL) {
    Word preprocessword = DefinitionTable_TokToWord(self->definition_table, PREPROCESSWORD);
    Environment_PushExecution(self,preprocessword);
    Environment_ForciblyEvaluate(self);
  }
}

// Man, this is becoming messier than I originally anticipated XD