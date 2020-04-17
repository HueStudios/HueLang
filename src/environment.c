#if INTERFACE
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "dictionary.h"
#include "doublylinked.h"
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
  DoublyLinkedList *word_buffer;
  unsigned char running;
  unsigned int comment;
  EnvironmentState state;
} Environment; 
#endif

#include "environment.h"

void create_environment() {
  Environment *self = malloc(sizeof(Environment));
  self->execution_stack = create_stack();
  self->value_stack = create_stack();
  self->state_stack = create_stack();
  self->dictionaries = malloc(sizeof(Dictionary*) * 3);
  self->dictionaries[0] = create_dictionary();
  self->dictionaries[1] = create_dictionary();
  self->dictionaries[2] = create_dictionary();
  self->word_buffer = create_doublylinked();
  self->comment = 0;
  self->running = 1;
  return self;
}

void execute_in_environment(Environment *self, char *word) {
  push_stack(self->execution_stack, word);
  run_environment(self);
}

void execute_multiple_in_environment(Environment *self, DoublyLinkedList *words) {
  for (DoublyLinkedListNode *focus = words->head; 
      focus != NULL; focus = focus->next) {
      if (focus == words->head) continue;
      if (focus == words->tail) continue;
      ArbitraryValue *val = focus->value;
      char *represented_word = val->value;
      execute_in_environment(self, represented_word);
  }
}

unsigned char eval_in_environment(Environment *self, char* word) {
  if (self->state == compilestate) {
    if (contains_key_dictionary(self->dictionaries[compiledictionary], 
      word)) {
      void (*word_function)(Environment*) 
        = get_value_dictionary(self->dictionaries[compiledictionary], word);
      word_function(self);
      return 1;
    }
  }

  if (self->state == normalstate) {
    if (contains_key_dictionary(self->dictionaries[primarydictionary], 
      word)) {
      void (*word_function)(Environment*) 
        = get_value_dictionary(self->dictionaries[primarydictionary], word);
      word_function(self);
      return 1;
    }
    else if (contains_key_dictionary(self->dictionaries[secondarydictionary], 
      word)) {
      for (DoublyLinkedListNode *focus = self->word_buffer->head; 
        focus != NULL; focus = focus->next) {
        if (focus == self->word_buffer->head) continue;
        if (focus == self->word_buffer->tail) continue;
        ArbitraryValue *val = focus->value;
        char *represented_word = val->value;
        push_stack(self->execution_stack, represented_word);
      }
      return 1;
    } else {
      ArbitraryValue *value = token_to_whatever(word, 1);
      if (value->type != symboltype) {
        push_stack(self->value_stack, value);
      } else {
        free(value->value);
        free(value);
      }
      return 1;
    }
  }
  return 0;
}

void run_environment(Environment *self) {
  while(self->execution_stack->size != 0) {
    char *word = pop_stack(self->execution_stack);

    if (word[0] == '(') { self->comment++; return; }
    if (word[0] == ')') { 
      self->comment--;
      if (self->comment < 0) {
        printf("Unbalanced comments.\n");
        return;
      }
    }
    if (self->comment > 0) return;

    if (self->state == compilestate) {
      if (!eval_in_environment(self, word)) {
        ArbitraryValue *value = token_to_whatever(word, 0);
        append_doublylinked(self->word_buffer, value);
      }      
    }

    if (self->state == hardcompilestate) {
      ArbitraryValue *value = malloc(sizeof(ArbitraryValue));
      value->value = token_to_symbol(word);
      value->type = symboltype;
      append_doublylinked(self->word_buffer, value);
      if (strcmp(word, "end")) {
        pop_stack(self->state_stack);
      }
    }

    if (self->state == normalstate) {
      if (!eval_in_environment(self, word)) {
        printf("Unknown word %s\n", word);
      }
    }
  }
}