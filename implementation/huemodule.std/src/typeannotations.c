#if INTERFACE

#include "definition.h"
#include "environment.h"
#include "utils.h"
#define SUBSETTABLEPREFIX "__subsets_"
#define SUBSET_BUCKET_COUNT 0xF

typedef struct SubsetTableEntry {
  Word type;
  SubsetTableEntry *next;
} SubsetTableEntry;

typedef struct SubsetTableBucket {
  unsigned int size;
  SubsetTableEntry *head;
} SubsetTableBucket;

typedef struct SubsetTable {
  SubsetTableBucket buckets[SUBSET_BUCKET_COUNT + 1];
} SubsetTable;

#endif

#include "typeannotations.h"

SubsetTable *SubsetTable_Create() {
  SubsetTable *self = malloc(sizeof(SubsetTable));
  for (unsigned int i = 0; i <= SUBSET_BUCKET_COUNT; i++) {
    self->buckets[i].size = 0;
    self->buckets[i].head = NULL;
  }
  return self;
}

void SubsetTable_AddSubset (SubsetTable *self, Environment *env,
  Word subset) {
  char *subset_name = DefinitionTable_GetName(env->definition_table, subset);
  unsigned long long hash = String_HashForTable(subset_name);
  unsigned char bucket_index = hash & SUBSET_BUCKET_COUNT;
  SubsetTableBucket *bucket = &self->buckets[bucket_index];
  SubsetTableEntry *new_entry = malloc(sizeof(SubsetTableEntry));
  new_entry->next = bucket->head;
  new_entry->type = subset;
  bucket->head = new_entry;
  bucket->size++;
}

unsigned char TypeAnnotations_HasSubsetType(Environment *env, Word set,
  Word subset) {
  if (set.major == subset.major && set.minor == subset.minor) {
    return 1;
  }
  char *set_name = DefinitionTable_GetName(env->definition_table, set);
  char *subset_table_name = Utils_MallocConcat(SUBSETTABLEPREFIX, set_name);
  Word subset_table_word = DefinitionTable_TokToWord(env->definition_table,
    subset_table_name);
  free(subset_table_name);
  Definition subset_table_def = DefinitionTable_GetDefinition(
    env->definition_table,
    subset_table_word
  );
  Word intflagword = DefinitionTable_TokToWord(env->definition_table,
    INTSTATEFLAG);
  if (subset_table_def.type.major != intflagword.major ||
    subset_table_def.type.minor != intflagword.minor) {
    return 0;
  }
  SubsetTable *table = subset_table_def.value.pointer;

  char *subset_name = DefinitionTable_GetName(env->definition_table, subset);
  unsigned long long hash = String_HashForTable(subset_name);
  unsigned char bucket_index = hash & SUBSET_BUCKET_COUNT;
  SubsetTableBucket *bucket = &table->buckets[bucket_index];
  for (SubsetTableEntry *focus = bucket->head; focus != NULL;
    focus = focus->next) {
    Word type = focus->type;
    if (TypeAnnotations_HasSubsetType(env, type, subset)) {
      return 1;
    }
  }
  return 0;
}

void TypeAnnotations_AddSubsetType(Environment *env, Word set, Word subset) {
  char *set_name = DefinitionTable_GetName(env->definition_table, set);
  char *subset_table_name = Utils_MallocConcat(SUBSETTABLEPREFIX, set_name);
  Word subset_table_word = DefinitionTable_TokToWord(env->definition_table,
    subset_table_name);
  free(subset_table_name);
  Definition subset_table_def = DefinitionTable_GetDefinition(
    env->definition_table,
    subset_table_word
  );
  Word intflagword = DefinitionTable_TokToWord(env->definition_table,
    INTSTATEFLAG);
  if (subset_table_def.type.major != intflagword.major ||
    subset_table_def.type.minor != intflagword.minor) {
    subset_table_def.type = intflagword;
    subset_table_def.value.pointer = SubsetTable_Create();
    DefinitionTable_SetDefinition(env->definition_table,
      subset_table_word, subset_table_def);
  }
  SubsetTable *table = subset_table_def.value.pointer;
  SubsetTable_AddSubset(table, env, subset);
}
