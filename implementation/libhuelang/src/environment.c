#if INTERFACE

typedef struct ValueStack ValueStack;

#include <signal.h>
#include <stdlib.h>
#include "definition.h"
#include "type.h"
#include "core.h"
#include "values.h"

#define PRIMARYDEFINITIONWORD "primary"
#define PREEVALWORD "preeval"

typedef struct ExecutionStackNode {
  Word value;
  ExecutionStackNode *next;
} ExecutionStackNode;

typedef struct Environment {
  DefinitionTable *definition_table;
  ExecutionStackNode *execution_stack;
  ValueStack *value_stack;
} Environment;

#endif

#include "environment.h"

// Create a runtime environment and return a pointer to it.
Environment *Environment_Create() {
  Environment *self = malloc(sizeof(Environment));
  self->definition_table = DefinitionTable_Create();
  self->value_stack = ValueStack_Create();

  Core_Initialize(self);

  return self;
}

// Helper function to quickly add a primary definition
void Environment_AddPrimaryDefinition (Environment *self,
  Word word, void (*definition)(Environment *)) {
  Word primaryword = DefinitionTable_TokToWord(self->definition_table,
    PRIMARYDEFINITIONWORD);
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
  volatile Word word = Environment_PeekExecution(self);
  Word primarydefinitiontype =
    DefinitionTable_TokToWord(self->definition_table, PRIMARYDEFINITIONWORD);
  // If the word is 'primary':
  if ((primarydefinitiontype.major == word.major) &
    (primarydefinitiontype.minor == word.minor)) {
    //  Call the handler of primary to handle the evaluation
    Definition primarydefinition =
      DefinitionTable_GetDefinition(self->definition_table,
        primarydefinitiontype);
    void (*defhandler)(Environment*) = primarydefinition.value.pointer;
    defhandler(self);
  } else {
    // Obtain the definition of the word.
    Definition worddef = DefinitionTable_GetDefinition(self->definition_table,
      word);
    // Obtain the definition type.
    Word definitiontype = worddef.type;
    //  Push the type to the execution stack
    Environment_PushExecution(self, definitiontype);
    //  Call forced evaluation
    Environment_Evaluate(self);
  }
}

// Enter the execution loop.
void Environment_Run(Environment *self) {
  while (self->execution_stack != NULL) {
    // Eval preeval for syntax extension
    Word preevalword =
      DefinitionTable_TokToWord(self->definition_table, PREEVALWORD);
    //raise(SIGINT);
    Environment_PushExecution(self, preevalword);
    Environment_Evaluate(self);
    // Evaluate the word itself
    Environment_Evaluate(self);
  }
}

// Man, this is becoming messier than I originally anticipated XD
