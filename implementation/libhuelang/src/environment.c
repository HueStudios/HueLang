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

// Helper function to quickly add a primary definition 
void Environment_AddPrimaryDefinition (Environment *self,
  Word word, void (*definition)(Environment *)) {
  Word primaryword = DefinitionTable_TokToWord(self->definition_table, PRIMARYDEFINITIONWORD);
  Definition thisdef;
  thisdef.type = primaryword;
  thisdef.value.pointer = definition;
  DefinitionTable_SetDefinition(self->definition_table, word, thisdef);
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

// Evaluate a word in this environment.
void Environment_Evaluate(Environment *self) {
  if (self->execution_stack == NULL) return;
  Word word = Environment_PeekExecution(self);
  // Obtain the definition of the word.
  Definition worddef = DefinitionTable_GetDefinition(self->definition_table, word);
  // Obtain the type of the definition
  Word primarydefinitiontype = DefinitionTable_TokToWord(self->definition_table, PRIMARYDEFINITIONWORD);
  Word definitiontype = worddef.type;
  // If the type is primary:
  if ((primarydefinitiontype.major == definitiontype.major) & 
    (primarydefinitiontype.minor == definitiontype.minor)) {
    //  Call the handler of primary to handle the evaluation
    Word primarydefinitiontype = DefinitionTable_TokToWord(self->definition_table, PRIMARYDEFINITIONWORD);
    Definition primarydefinition = DefinitionTable_GetDefinition(self->definition_table, primarydefinitiontype);
    void (*defhandler)(Environment*) = primarydefinition.value.pointer;
    defhandler(self);
  } else {
    //  Push the type to the execution stack
    Environment_PushExecution(self, definitiontype);
    //  Call forced evaluation
    Environment_Evaluate(self);
  }
} 

// Enter the execution loop.
void Environment_Run(Environment *self) {
  while (self->execution_stack != NULL) {
    // Evaluate it
    Environment_Evaluate(self);
  }
}

// Man, this is becoming messier than I originally anticipated XD