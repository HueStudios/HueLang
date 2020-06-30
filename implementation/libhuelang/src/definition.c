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
  DefinitionTableBucket buckets[DEF_BUCKET_COUNT + 1];
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
  for (unsigned int i = 0; i <= DEF_BUCKET_COUNT; i++) {
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

// Get the name of a word.
char *DefinitionTable_GetName(DefinitionTable *self, Word word) {
  return self->buckets[word.major].entries[word.minor].name;
}

// Convert a token to a word. Also registers the word when needed.
Word DefinitionTable_TokToWord(DefinitionTable *self,
  char *token) {
  Word result;
  unsigned long long hash = String_HashForTable(token);
  unsigned int bucket_index = hash & DEF_BUCKET_COUNT;
  result.major = bucket_index;
  // Try to find the token in its corresponding bucket.
  DefinitionTableBucket *bucket = &self->buckets[bucket_index];
  for (unsigned long i = 0; i < bucket->size; i++) {
    DefinitionTableEntry entry = bucket->entries[i];
    if (strcmp(entry.name, token) == 0) {
      // A word already has this token as its name, so
      // it is returned.
      result.minor = i;
      return result;
    }
  }
  // No word has yet been registered with this token as
  // its name. Therefore we must register it.
  if (bucket->size == bucket->max_size) {
    bucket->max_size <<= 1;
    bucket->entries = realloc(bucket->entries, bucket->max_size);
  }
  bucket->entries[bucket->size].name = malloc(sizeof(char) *
    (strlen(token) + 1));
  strcpy(bucket->entries[bucket->size].name, token);
  result.minor = bucket->size;

  // Set the default definition of the word to none.
  if (strcmp(token, UNDEFINEDWORD) != 0) {
    Definition defaultdef;
    defaultdef.value.number = 0;
    defaultdef.type = DefinitionTable_TokToWord(self, UNDEFINEDWORD);
    bucket->entries[bucket->size].definition = defaultdef;
  }

  bucket->size += 1;

  return result;
}

// Create a new word with a random name, sharing the same definition as
// an original.
Word DefinitionTable_MakeAnonymousDefinition (DefinitionTable *self,
  Word original) {
  char *original_name = DefinitionTable_GetName(self, original);
  unsigned int original_len = strlen(original_name);
  unsigned int new_len = original_len + 1 + 16 + 1;
  char *new_name = malloc(sizeof(char) * new_len);
  for (int i = 0; i < new_len - 1; i++) {
    if (i < original_len) {
      new_name[i] = original_name[i];
    }
    if (i == original_len) {
      new_name[i] = '_';
    }
    if (i > original_len) {
      new_name[i] = rand() % (126 + 1 - 33) + 33;
    }
  }
  new_name[new_len-1] = '\0';
  Word result = DefinitionTable_TokToWord(self, new_name);
  Definition def = DefinitionTable_GetDefinition(self, original);
  DefinitionTable_SetDefinition(self, result, def);
  free(new_name);
  return result;
}
