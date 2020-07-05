#if INTERFACE

typedef struct Word {
  short major;
  long minor;
} Word;

typedef struct WordLinkedListNode {
  Word word;
  WordLinkedListNode *next;
} WordLinkedListNode;

typedef struct WordLinkedList {
  unsigned int size;
  WordLinkedListNode *head;
} WordLinkedList;


#endif

#include "word.h"
