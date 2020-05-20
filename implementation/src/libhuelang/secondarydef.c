#if INTERFACE

#include "word.h"

typedef struct SecondaryDefinition {
  Word *data;
  int size;
} SecondaryDefinition;

typedef struct SecondaryDefinitionBuffer {
  SecondaryDefinitionBufferNode *head;
  unsigned int size;
} SecondaryDefinitionBuffer;

typedef struct SecondaryDefinitionBufferNode {
  Word value;
  SecondaryDefinitionBuffer *next;
} SecondaryDefinitionBufferNode;

#endif

#include "secondarydef.h"