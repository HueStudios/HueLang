#if INTERFACE

#include <string.h>
#include <stdlib.h>
#include "word.h"

#define INITIAL_DEF_BUCKET_SIZE 2
#define DEF_BUCKET_COUNT 0xFFF
#define UNDEFINEDWORD "undefined"

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
      = calloc(INITIAL_DEF_BUCKET_SIZE,sizeof(DefinitionTableEntry));
  }
  return self;
}

// Compute the hash of a string to be used on a hashtable.
unsigned long long String_HashForTable (char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

// Set the definition of a word.
void DefinitionTable_SetDefinition(DefinitionTable *self, Word word, 
  Definition definition) {
  self->buckets[word.major].entries[word.minor].definition = definition;
}

// Get the definition of a word. 
Definition DefinitionTable_GetDefinition(DefinitionTable *self, Word word) {
  return self->buckets[word.major].entries[word.minor].definition;
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
  bucket.entries[bucket.size].name = malloc(sizeof(char) * (strlen(token) + 1));
  strcpy(bucket.entries[bucket.size].name, token);
  result.minor = bucket.size;
  bucket.size += 1;

  // Set the default definition of the word to none.
  if (strcmp(token, UNDEFINEDWORD) != 0) {
    Definition defaultdef;
    defaultdef.value.number = 0;
    defaultdef.type = DefinitionTable_TokToWord(self, UNDEFINEDWORD);
    bucket.entries[bucket.size].definition = defaultdef;
  }

  // Commit changes to the bucket
  self->buckets[bucket_index] = bucket;  

  return result;
}