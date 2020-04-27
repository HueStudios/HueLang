#if INTERFACE
#include <stdlib.h>
typedef struct LinkedList {
  DoublyLinkedListNode *head;
  unsigned int size;
} DoublyLinkedList;
typedef struct LinkedListNode {
  DoublyLinkedListNode *next;
  void *value;
} DoublyLinkedListNode;
#endif

#include "linked.h"

LinkedList *create_linked() {
  LinkedList *result = malloc(sizeof(LinkedList));
  result->head = NULL;
  result->size = 0;
  return result;
}

void preppend_linked(LinkedList *self, void *value) {
  LinkedListNode *toappend = malloc(sizeof(LinkedListNode));
  toappend->value = value;
  toappend->next = self->head;
  self->head = toappend;
}