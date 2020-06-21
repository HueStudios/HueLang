#if INTERFACE

#include <stdlib.h>
#include "definition.h"
#include "type.h"
#include "core.h"

#define PREPROCESSWORD "!preprocess"
#define DEFAULTRESOLVEWORD "!defaultresolve"
#define PRIMARYDEFINITIONWORD "primary"

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

  Core_Initialize(self);

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
  Word primarydefinitiontype = DefinitionTable_TokToWord(self->definition_table, PRIMARYDEFINITIONWORD);
  Definition primarydefinition = DefinitionTable_GetDefinition(self->definition_table, primarydefinitiontype);
  // Call the primary definition handler.
  void (*defhandler)(Environment*) = primarydefinition.value.pointer;
  defhandler(self);
} 

// Enter the execution loop.
void Environment_Run(Environment *self) {
  while (self->execution_stack != NULL) {
    Word preprocessword = DefinitionTable_TokToWord(self->definition_table, PREPROCESSWORD);
    Environment_PushExecution(self,preprocessword);
    Environment_ForciblyEvaluate(self);
    // Get the word
    Word word = Environment_PeekExecution(self);
    // Obtain the definition of the word.
    Definition worddef = DefinitionTable_GetDefinition(self->definition_table, word);
    // Obtain the type of the definition
    Word definitiontype = worddef.type;
    // Verify if the word has a definition.
    Word undefinedword = DefinitionTable_TokToWord(self->definition_table, UNDEFINEDWORD);
    if ((definitiontype.major==undefinedword.major)&
      (definitiontype.minor==undefinedword.minor)) {
      // Evaluate default resolution.
      Word defaultresolveword = DefinitionTable_TokToWord(self->definition_table, DEFAULTRESOLVEWORD);
      Environment_PushExecution(self,defaultresolveword);
      Environment_ForciblyEvaluate(self);
    } else {
      // Evaluate the word itself.
      Environment_ForciblyEvaluate(self);
    }
  }
}

// Man, this is becoming messier than I originally anticipated XD