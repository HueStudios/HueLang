#if INTERFACE
#include <stdlib.h>
#include <string.h>
typedef struct Dictionary {
  DictionaryBin **bins;
  int bin_count;
} Dictionary;
typedef struct DictionaryBin {
  DictionaryEntry *head;
  int entry_count;
}
typedef struct DictionaryEntry {
  char *key;
  void *value;
  DictionaryEntry *next;
}
#endif

#include "dictionary.h"

unsigned long hash(unsigned char *str) {
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

Dictionary *create_dictionary () {
  Dictionary *self = malloc(sizeof(Dictionary));
  self->bin_count = 0xFFFF; // Must be a power of 2 for masking.
  self->bins = malloc(sizeof(DictionaryBin*) * self->bin_count);
  for (int i = 0; i < self->bin_count; i++) { // Also, make sure 
  // the type of i won't overflow
    self->bins[i] = malloc(sizeof(DictionaryBin));
    self->bins[i]->head = NULL;
    self->bins[i]->entry_count = 0;
  }
  return self;
}

DictionaryBin *get_bin_dictionary(Dictionary *self, char *key) {
  unsigned long bin_id = hash(key) & self->bin_count; 
  DictionaryBin *bin = self->bins[bin_id];
  return bin;
}

DictionaryEntry *get_entry_dictionarybin(DictionaryBin *self, char *key) {
  DictionaryEntry *result = NULL;
  for (DictionaryEntry *focus = self->head; focus != NULL; 
    focus = focus->next) {
    if (strcmp(focus->key,key) == 0) {
      result = focus;
    }
  }
  return result;
}

void *get_value_dictionary(Dictionary *self, char *key) {
  DictionaryBin *bin = get_bin_dictionary(self, key);
  void *result = NULL;
  DictionaryEntry *entry = get_entry_dictionarybin(bin, key);
  if (entry != NULL) {
    result = entry->value;
  }
  return result;
}

void set_value_dictionary(Dictionary *self, char *key, void *value) {
  DictionaryBin *bin = get_bin_dictionary(self, key);
  DictionaryEntry *entry = get_entry_dictionarybin(bin, key);
  if (entry != NULL) {
    entry->value = value;
  } else {
    DictionaryEntry *new_entry = malloc(sizeof(DictionaryEntry));
    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = bin->head;
    bin->head = new_entry;
  }
}

unsigned char contains_key_dictionary(Dictionary *self, char *key) {
  DictionaryBin *bin = get_bin_dictionary(self, key);
  DictionaryEntry *entry = get_entry_dictionarybin(bin, key);
  return (entry != NULL);
}