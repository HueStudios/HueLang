#if INTERFACE

#include "definition.h"
#include "environment.h"
#include "utils.h"
#include <math.h>
#include <float.h>
#include <limits.h>
#define SUBSETTABLEPREFIX "__subsets_"
#define SUBSET_BUCKET_COUNT 0xF

#define TYPEANNOTATEDWORD "typeannotated"

typedef struct SubsetTable {
  WordLinkedList buckets[SUBSET_BUCKET_COUNT + 1];
} SubsetTable;

typedef struct AnnotationPair {
  WordArrayList *annotation;
  WordArrayList *production;
  Word tocall;
}

typedef struct TypeAnnotatedDefinition {
  AnnotationPair *pairs;
  unsigned int size;
  unsigned int max_size;
}

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

double TypeAnnotations_AnnotationDistance(Environment *env, WordArrayList
  *annotation) {
  double accum = 0.0;
  Word *st_focus = (ValueStack_GetAbsolutePointer(env->value_stack)
    - sizeof(Word));

  for (unsigned int i = 0; i < annotation->size; i++) {
    if ((void*)st_focus < env->value_stack->data) {
      return -1.0;
    }

    Word ver_word = *st_focus;
    Word ann_word = annotation->data[i];

    unsigned short distance1d = TypeAnnotations_SubsetDistance(env, ann_word,
      ver_word);
    if (distance1d == USHRT_MAX) {
      return -1.0;
    }
    accum += ((double)distance1d)*((double)distance1d);

    st_focus =
      (Word*)(((void*)st_focus) - Types_ResolveTypeSize(env,st_focus)) - 1;
  }
  return accum;
}

void TypeAnnotations_MakeDefinition(Environment *env, Word
  word) {
  Word typeannotatedword = DefinitionTable_TokToWord(env->definition_table,
    TYPEANNOTATEDWORD);

  Definition priori = DefinitionTable_GetDefinition(env->definition_table,
    word);
  if (priori.type.major == typeannotatedword.major
    && priori.type.minor == typeannotatedword.minor) {
    // The word already has a type annotated definition.
    return;
  }

  TypeAnnotatedDefinition *tadef = malloc(sizeof(TypeAnnotatedDefinition));
  tadef->size = 0;
  tadef->max_size = 1;
  tadef->pairs = malloc(sizeof(AnnotationPair));

  Definition newdef;
  newdef.type = typeannotatedword;
  newdef.value.pointer = tadef;

  DefinitionTable_SetDefinition(env->definition_table, word,
    newdef);
}

void TypeAnnotations_AddEntry(Environment *env, Word target, Word tocall,
  WordArrayList *annotation, WordArrayList *production) {

  Word typeannotatedword = DefinitionTable_TokToWord(env->definition_table,
    TYPEANNOTATEDWORD);
  Definition defcont = DefinitionTable_GetDefinition(env->definition_table,
    target);

  if (defcont.type.major != typeannotatedword.major ||
    defcont.type.minor != typeannotatedword.minor) {
    // ERROR can't add entry to a non-annotated definition.
    return;
  }

  TypeAnnotatedDefinition *tadef = defcont.value.pointer;

  for (unsigned int i = 0; i < tadef->size; i++) {
    AnnotationPair pair = tadef->pairs[i];
    WordArrayList *thisAnnotation = pair.annotation;
    if (thisAnnotation->size == annotation->size) {
      unsigned char matching = 1;
      for (unsigned int j = 0; j < annotation->size; j++) {
        Word focusWord = thisAnnotation->data[j];
        Word otherWord = annotation->data[j];
        if (focusWord.major != otherWord.major ||
          focusWord.minor != otherWord.minor) {
          matching = 0;
          break;
        }
      }
      if (matching) {
        free(tadef->pairs[i].production->data);
        free(tadef->pairs[i].production);
        tadef->pairs[i].production = production;
        tadef->pairs[i].tocall = tocall;
        return;
      }
    }
  }

  AnnotationPair new_pair;
  new_pair.annotation = annotation;
  new_pair.production = production;
  new_pair.tocall = tocall;

  tadef->pairs[tadef->size] = new_pair;
  if (tadef->size == tadef->max_size) {
    tadef->max_size *= 2;
    tadef->pairs = realloc(tadef->pairs, tadef->max_size);
  }
}

void __typeannotated(Environment *env) {
  if (env->execution_stack == NULL) {
    return;
  }
  Word word = Environment_PopExecution(env);
  // Obtain the definition of the word.
  Definition worddef = DefinitionTable_GetDefinition(env->definition_table,
    word);

  TypeAnnotatedDefinition *annodef = worddef.value.pointer;

  double min_score = DBL_MAX;
  unsigned int max_param = 0;
  unsigned int best_index = 0;

  for (unsigned int i = 0; i < annodef->size; i++) {
    if (annodef->pairs[i].annotation->size < max_param) continue;

    double this_score = TypeAnnotations_AnnotationDistance(env,
      annodef->pairs[i].annotation);

    if (this_score == -1.0) continue;

    if (annodef->pairs[i].annotation->size > max_param ||
      this_score < min_score) {
      max_param = annodef->pairs[i].annotation->size;
      min_score = this_score;
      best_index = i;
    }
  }

  if (min_score != DBL_MAX) {
    Environment_PushExecution(env, annodef->pairs[best_index].tocall);
  }
}
