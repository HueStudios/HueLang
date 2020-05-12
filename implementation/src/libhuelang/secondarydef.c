#if INTERFACE

typedef struct SecondaryDefinition {
  word *data;
  int size;
} SecondaryDefinition;

typedef struct SecondaryDefinitionBuffer {
  SecondaryDefinitonBufferNode *head;
  unsigned int size;
}

typedef struct SecondaryDefinitionBufferNode {
  word value;
  SecondaryDefinitionBuffer *next;
}

#endif

#include "secondarydef.h"