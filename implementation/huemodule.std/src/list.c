#if INTERFACE

#include "environment.h"
#include "values.h"
#include "type.h"

#define LISTWORD "list"
#define LISTSTARTWORD "{"
#define LISTENDWORD "}"
#define LISTDELIMITERWORD "__listdelimiter"

#endif

#include "list.h"

unsigned long __list_typehandler(Environment *env, Word* word_pointer) {
  unsigned long *size_pointer = (unsigned long*)(word_pointer) - 1;
  return *size_pointer;
}

void __liststart(Environment *env) {
  Word listdelimiterword = DefinitionTable_TokToWord(env->definition_table,
    LISTDELIMITERWORD);
  Word *salloc = ValueStack_PushAlloc(env->value_stack, sizeof(Word));
  *salloc = listdelimiterword;
}

void __listend(Environment *env) {
  Word listdelimiterword = DefinitionTable_TokToWord(env->definition_table,
    LISTDELIMITERWORD);
  Word listword = DefinitionTable_TokToWord(env->definition_table,
    LISTWORD);
  unsigned long count = 0;
  Word *focus;
  for (
    focus = (ValueStack_GetAbsolutePointer(env->value_stack) - sizeof(Word));
    (focus->major != listdelimiterword.major ||
    focus->minor != listdelimiterword.minor) &&
    (void*)focus >= env->value_stack->data;
    focus = (Word*)(((void*)focus) - Types_ResolveTypeSize(env,focus)) - 1
  ) {
    count++;
  }
  void *salloc = ValueStack_PushAlloc
    (env->value_stack, (sizeof(unsigned long) * 2) + sizeof(Word));
  unsigned long *count_ptr = salloc;
  unsigned long *size_ptr = count_ptr + 1;
  unsigned long *header_ptr_i = size_ptr + 1;
  Word *header_ptr = (Word*)header_ptr_i;
  unsigned long size =
    ((unsigned long)ValueStack_GetAbsolutePointer(env->value_stack))-
    ((unsigned long)focus) - sizeof(Word);
  *count_ptr = count;
  *size_ptr = size;
  *header_ptr = listword;
}

void Lists_Initialize(Environment *env) {
  Word listdelimiterword = DefinitionTable_TokToWord(env->definition_table,
    LISTDELIMITERWORD);
  Types_RegisterAtomicType(env, listdelimiterword, 0);

  Word listword = DefinitionTable_TokToWord(env->definition_table,
    LISTWORD);
  Types_RegisterCompoundType(env, listword, &__list_typehandler);

  Word lstartword = DefinitionTable_TokToWord(env->definition_table,
    LISTSTARTWORD);
  Environment_AddPrimaryDefinition(env, lstartword, &__liststart);

  Word lendword = DefinitionTable_TokToWord(env->definition_table,
    LISTENDWORD);
  Environment_AddPrimaryDefinition(env, lendword, &__listend);
}
