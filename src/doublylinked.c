#if INTERFACE
#include <stdlib.h>
typedef struct DoublyLinkedList {
  DoublyLinkedListNode *head;
  DoublyLinkedListNode *tail;
  unsigned int size;
} DoublyLinkedList;
typedef struct DoublyLinkedListNode {
  DoublyLinkedListNode *next;
  DoublyLinkedListNode *previous;
  void *value;
} DoublyLinkedListNode;
#endif

#include "doublylinked.h"

DoublyLinkedList *create_doublylinked() {
  DoublyLinkedList *result = malloc(sizeof(DoublyLinkedList));
  result->head = NULL;
  result->tail = NULL;
  result->size = 0;
  return result;
}

void preppend_doublylinked(DoublyLinkedList *self, void *value) {
  DoublyLinkedListNode *toappend = malloc(sizeof(DoublyLinkedListNode));
  toappend->value = value;
  toappend->next = NULL;

  // Do relinking and stuff
  toappend->previous = self->tail;
  self->tail->next = toappend;
  self->tail = toappend;

  self->size++;
}