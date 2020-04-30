#if INTERFACE

#include "linkedlist.h"
#include "environment.h"

typedef union DefinitionValue {
  void (*primary)(Environment*);
  LinkedList *secondary;
}

typedef struct Definition {
  DefinitionValue value;
  unsigned char is_primary;
} Definition;

#endif

#include "definition.h"