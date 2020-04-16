
#if INTERFACE
#include <stdlib.h>
typedef struct Stack {
  int size;
  StackNode *head;
} Stack;

typedef struct StackNode {
  StackNode *next;
  void *data;
} StackNode;
#endif

#include "stack.h"

Stack *create_stack() {
  Stack *self = malloc(sizeof(Stack));
  self->size = 0;
  self->head = NULL;
  return self;
}

void *pop_stack(Stack *self) {
  if (self->size == 0) {
    return NULL;
  }
  StackNode *new_head = self->head->next;
  void *result = self->head->data;
  free(self->head);
  self->head = new_head;
  self->size--;
  return result;
}

void push_stack(Stack *self, void *data) {
  StackNode *new_head = malloc(sizeof(StackNode));
  new_head->data = data;
  new_head->next = self->head;
  self->head = new_head;
  self->size++;
}