#if INTERFACE

#include "definition.h"
#include "environment.h"
#include "utils.h"
#include <math.h>
#include <limits.h>
#define SUBSETTABLEPREFIX "__subsets_"
#define SUBSET_BUCKET_COUNT 0xF

typedef struct SubsetTable {
  WordLinkedList buckets[SUBSET_BUCKET_COUNT + 1];
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
  WordLinkedList *bucket = &self->buckets[bucket_index];
  WordLinkedListNode *new_entry = malloc(sizeof(WordLinkedListNode));
  new_entry->next = bucket->head;
  new_entry->word = subset;
  bucket->head = new_entry;
  bucket->size++;
}

unsigned short TypeAnnotations_SubsetDistance(Environment *env, Word set,
  Word subset) {
  if (set.major == subset.major && set.minor == subset.minor) {
    return 0;
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
    return USHRT_MAX;
  }
  SubsetTable *table = subset_table_def.value.pointer;

  unsigned short min = USHRT_MAX;
  for (unsigned char bucket_index = 0; bucket_index <= SUBSET_BUCKET_COUNT;
    bucket_index++) {
    WordLinkedList *bucket = &table->buckets[bucket_index];
    for (WordLinkedListNode *focus = bucket->head; focus != NULL;
      focus = focus->next) {
      Word type = focus->word;
      unsigned short in_result =
        TypeAnnotations_SubsetDistance(env, type, subset);
      if (in_result != USHRT_MAX) in_result++;
      if (in_result < min) {
        min = in_result;
      }
    }
  }
  return min;
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

double TypeAnnotations_AnnotationDistance(Environment *env, WordLinkedList
  *annotation) {
  double accum = 0.0;
  WordLinkedListNode *ann_focus;
  Word *st_focus = (ValueStack_GetAbsolutePointer(env->value_stack)
    - sizeof(Word));

  for (ann_focus = annotation->head; ann_focus != NULL;
    ann_focus = ann_focus->next) {
    if ((void*)st_focus < env->value_stack->data) {
      return -1.0;
    }

    Word ver_word = *st_focus;
    Word ann_word = ann_focus->word;

    unsigned short distance1d = TypeAnnotations_SubsetDistance(env, ann_word,
      ver_word);
    if (distance1d == USHRT_MAX) {
      return -1.0;
    }
    accum += ((double)distance1d)*((double)distance1d);

    st_focus =
      (Word*)(((void*)st_focus) - Types_ResolveTypeSize(env,st_focus)) - 1;
  }
  return sqrt(accum);
}
