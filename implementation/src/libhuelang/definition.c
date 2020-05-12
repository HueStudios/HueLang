#if INTERFACE

#include "secondarydef.h"
#include "environment.h"
#include "word.h"

typedef union DefinitionValue {
  void (*primary)(Environment*);
  SecondaryDefinition *secondary;
}

typedef struct Definition {
  DefinitionValue value;
  unsigned char is_primary;
} Definition;

typedef struct DefinitionTable {
  DefinitionTableBucket buckets[0xFFF];
} DefinitionTable;

typedef struct DefinitionTableEntry {
  char *name;
  Definition definition;
}

typedef struct DefinitionTableBucket {
  unsigned int max_size;
  unsigned int size;
  DefinitionTableEntry *entries;
}

#define INITIAL_DEF_BUCKET_SIZE 2;

#endif

#include "definition.h"

DefinitionTable *DefinitionTable_Create() {
  DefinitionTable *self = malloc(sizeof(DefinitionTable));
  for (unsigned int i = 0; i < 0xFFF; i++) {
    self->buckets[i].max_size = 2;
    self->buckets[i].size = 0;
    self->buckets[i].entries 
      = malloc(sizeof(DefinitionTableEntry) 
                * INITIAL_DEF_BUCKET_SIZE);
  }
  return self;
}

Word DefinitionTable_RegisterWord(DefinitionTable *self, 
  char *token) {
  
}