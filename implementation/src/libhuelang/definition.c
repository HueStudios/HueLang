#if INTERFACE

#include <string.h>
#include "secondarydef.h"
#include "environment.h"
#include "word.h"

#define INITIAL_DEF_BUCKET_SIZE 2
#define DEF_BUCKET_COUNT 0xFFF

typedef union DefinitionValue {
  void *pointer;
  long number;
}

typedef struct Definition {
  DefinitionValue value;
  Word type;
} Definition;

typedef struct DefinitionTable {
  DefinitionTableBucket buckets[DEF_BUCKET_COUNT];
} DefinitionTable;

typedef struct DefinitionTableEntry {
  char *name;
  Definition definition;
}

typedef struct DefinitionTableBucket {
  unsigned long max_size;
  unsigned long size;
  DefinitionTableEntry *entries;
}

#endif

#include "definition.h"

// Create a definition table and return a pointer to it.
DefinitionTable *DefinitionTable_Create() {
  DefinitionTable *self = malloc(sizeof(DefinitionTable));
  // Initialize all buckets to empty lists. 
  for (unsigned int i = 0; i < DEF_BUCKET_COUNT; i++) {
    self->buckets[i].max_size = 2;
    self->buckets[i].size = 0;
    self->buckets[i].entries 
      = malloc(sizeof(DefinitionTableEntry) 
                * INITIAL_DEF_BUCKET_SIZE);
  }
  return self;
}

// Compute the hash of a string to be used on a hashtable.
unsigned long long String_HashForTable (char *str) {
  // TODO
  return 1;
}

// Convert a token to a word. Also registers the word when needed.
Word DefinitionTable_TokToWord(DefinitionTable *self, 
  char *token) {
  Word result;
  unsigned long long hash = String_HashForTable(token);
  unsigned int bucket_index = hash & DEF_BUCKET_COUNT;
  result.major = bucket_index;
  // Try to find the token in its corresponding bucket.
  DefinitionTableBucket bucket = self->buckets[bucket_index];
  for (unsigned long i = 0; i < bucket.size; i++) {
    DefinitionTableEntry entry = bucket.entries[i];
    if (strcmp(entry.name, token) == 0) {
      // A word already has this token as its name, so 
      // it is returned. 
      result.minor = i;
      return result;
    }
  }
  // No word has yet been registered with this token as 
  // its name. Therefore we must register it. 
  if (bucket.size == bucket.max_size) {
    bucket.max_size <<= 1;
    bucket.entries = realloc(bucket.entries, bucket.max_size);
  }
  bucket.size += 1;
  bucket.entries[bucket.size].name = strcpy(token);
  result.minor = bucket.size;
  return result;
}