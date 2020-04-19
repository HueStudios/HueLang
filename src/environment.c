#if INTERFACE
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "dictionary.h"
#include "linked.h"
#include "typeparsing.h"

typedef enum EnvironmentState {
  normalstate,
  compilestate,
  hardcompilestate
} EnvironmentState;

typedef enum DictionaryIndex {
  primarydictionary = 0,
  secondarydictionary = 1,
  compiledictionary = 2
} DictionaryIndex;

typedef struct Environment {
  Stack *execution_stack;
  Stack *value_stack;
  Stack *state_stack;
  Dictionary **dictionaries;
  LinkedList *word_buffer;
  char *word_name;
  unsigned char running;
  unsigned int comment;
} Environment; 
#endif

#include "environment.h"

Environment *create_environment() {
  Environment *self = malloc(sizeof(Environment));
  self->execution_stack = create_stack();
  self->value_stack = create_stack();
  self->state_stack = create_stack();
  self->dictionaries = malloc(sizeof(Dictionary*) * 3);
  self->dictionaries[0] = create_dictionary();
  self->dictionaries[1] = create_dictionary();
  self->dictionaries[2] = create_dictionary();
  self->word_buffer = create_linked();
  self->comment = 0;
  self->running = 1;
  return self;
}

EnvironmentState get_state_environment(Environment *self) {
  if (self->state_stack->size == 0) {
    return normalstate;
  }
  return *(EnvironmentState*)peek_stack(self->state_stack);
}

EnvironmentState pop_state_environment(Environment *self) {
  EnvironmentState *val = pop_stack(self->state_stack);
  EnvironmentState result = *val;
  free(val);
  return result;
}

void push_state_environment(Environment *self, EnvironmentState state) {
  EnvironmentState *topush = malloc(sizeof(EnvironmentState));
  *topush = state;
  push_stack(self->state_stack, topush);
}

void execute_in_environment(Environment *self, char *word) {
  //printf("Executing %s\n", word);
  ArbitraryValue *word_val = malloc(sizeof(ArbitraryValue));
  word_val->value = word;
  word_val->dynamic = 1;
  word_val->type = symboltype;
  push_stack(self->execution_stack, word_val);
  run_environment(self);
}

unsigned char eval_in_environment(Environment *self, ArbitraryValue* word) {
  if (word->type != symboltype) {
    printf("Tried to evaluate a non-symbolic word!");
    return 0;
  }

  //printf("Evaluating word %s \n", word->value);

  if (get_state_environment(self) == compilestate) {
    if (contains_key_dictionary(self->dictionaries[compiledictionary], 
      word->value)) {
      void (*word_function)(Environment*) 
        = get_value_dictionary(self->dictionaries[compiledictionary], word->value);
      word_function(self);
      return 1;
    }
  }

  if (get_state_environment(self) == normalstate) {
    if (contains_key_dictionary(self->dictionaries[primarydictionary], 
      word->value)) {
      void (*word_function)(Environment*) 
        = get_value_dictionary(self->dictionaries[primarydictionary], word->value);
      word_function(self);
      //printf("Ran primary word %s\n", word->value);
      return 1;
    }
    else if (contains_key_dictionary(self->dictionaries[secondarydictionary], 
      word->value)) {
      //printf("Running secondary word named %s\n", word->value);
      LinkedList *word_definition = 
        get_value_dictionary(self->dictionaries[secondarydictionary], word->value);
      //printf("The word definition is at %p\n", word_definition);
      for (LinkedListNode *focus = word_definition->head; 
        focus != NULL; focus = focus->next) {
        ArbitraryValue *val = focus->value;
        //printf("Pushing %s into the execution stack\n", val->value);
        push_stack(self->execution_stack, val);
      }
      return 1;
    } else {
      ArbitraryValue *value = token_to_whatever(word->value, 1);
      if (value->type != symboltype) {
        push_stack(self->value_stack, value);
        //printf("Added value to value stack\n");
      } else {
        free(value->value);
        free(value);
        return 0;
      }
      return 1;
    }
  }
  return 0;
}

void run_environment(Environment *self) {
  //printf("Entered run mode\n");
  while(self->execution_stack->size != 0) {
    ArbitraryValue *word = pop_stack(self->execution_stack);

    if (word->type != symboltype) {
      //printf("A non-symbol value (%s) reached the execution stack!\n", word->value);
      return;
    }

    if (strcmp((char*)word->value, "(") == 0) { self->comment++; return; }
    if (strcmp((char*)word->value, ")") == 0) { 
      self->comment--;
      if (self->comment < 0) {
        printf("Unbalanced comments.\n");
        return;
      }
    }
    if (self->comment > 0) return;

    if (get_state_environment(self) == compilestate) {
      word->dynamic = 0;
      if (self->word_name == NULL) {
        self->word_name = word->value;
      } else if (!eval_in_environment(self, word)) {
        preppend_linked(self->word_buffer, word);
      }      
    } else if (get_state_environment(self) == hardcompilestate) {
      word->dynamic = 0;
      preppend_linked(self->word_buffer, word);
      if (strcmp((char*)word->value, "end") == 0) {
        pop_state_environment(self);
      }
    } else if (get_state_environment(self) == normalstate) {
      if (!eval_in_environment(self, word)) {
        printf("Unknown word %s\n", word->value);
      }
    }
    cleanup_value(word);
  }
}