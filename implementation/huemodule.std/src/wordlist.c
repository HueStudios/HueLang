#if INTERFACE

#include "environment.h"
#include "values.h"
#include "type.h"
#include "list.h"
#include <string.h>

#define WORDLISTSTARTWORD "["
#define WORDLISTENDWORD "]"
#define LISTDEPTHWORD "__listdepth"
#define WORDLISTPREEVALWORD "__wordlistpreeval"

#endif

#include "wordlist.h"

void __wordlist_preeval(Environment *env) {
  if (env->execution_stack == NULL) {
    return;
  }
  Word word = Environment_PopExecution(env);
  char *name = DefinitionTable_GetName(env->definition_table, word);
  Word listdepthword = DefinitionTable_TokToWord(env->definition_table,
    LISTDEPTHWORD);
  Definition listdepthworddef = DefinitionTable_GetDefinition(
    env->definition_table, listdepthword
  );
  unsigned int depth = listdepthworddef.value.number;
  unsigned char omit_quote = 0;
  if (strcmp(name,WORDLISTSTARTWORD)) {
    if (depth == 0) {
      omit_quote = 1;
    }
    depth++;
  }
  if (strcmp(name,WORDLISTENDWORD)) {
    depth--;
  }
  if (depth > 0 && !omit_quote) {
    unsigned int old_len = strlen(name);
    unsigned int new_len = old_len+1+1;
    char *new_name = malloc(new_len * sizeof(char));
    new_name[0] = '\0';
    strcat(new_name,":");
    strcat(new_name,name);
    Word newword = DefinitionTable_TokToWord(env->definition_table,
      new_name);
    free(new_name);
    Environment_PushExecution(env, newword);
  }
  if (listdepthworddef.value.number != depth) {
    listdepthworddef.value.number = depth;
    DefinitionTable_SetDefinition(env->definition_table,listdepthword,
      listdepthworddef);
  }
}

void __wordliststart(Environment *env) {
  Word liststartword = DefinitionTable_TokToWord(env->definition_table,
    LISTSTARTWORD);
  Environment_PushExecution(env, liststartword);
}

void __wordlistend(Environment *env) {
  Word listendword = DefinitionTable_TokToWord(env->definition_table,
    LISTENDWORD);
  Environment_PushExecution(env, listendword);
}

void WordLists_Initialize(Environment *env) {
  Word wlstartword = DefinitionTable_TokToWord(env->definition_table,
    WORDLISTSTARTWORD);
  Environment_AddPrimaryDefinition(env, wlstartword, &__wordliststart);

  Word wlendword = DefinitionTable_TokToWord(env->definition_table,
    WORDLISTENDWORD);
  Environment_AddPrimaryDefinition(env, wlendword, &__wordlistend);

  Word listdepthword = DefinitionTable_TokToWord(env->definition_table,
    LISTDEPTHWORD);
  Word intflagword = DefinitionTable_TokToWord(env->definition_table,
    INTSTATEFLAG);
  Definition listdepthworddef;
  listdepthworddef.type = intflagword;
  listdepthworddef.value.number = 0;
  DefinitionTable_SetDefinition(env->definition_table, listdepthword,
    listdepthworddef);

  Word wordlistpreeval = DefinitionTable_TokToWord(env->definition_table,
    WORDLISTPREEVALWORD);
  Environment_AddPrimaryDefinition(env, wordlistpreeval,
    &__wordlist_preeval);

  Word preevalword =
    DefinitionTable_TokToWord(env->definition_table, PREEVALWORD);
  Word old_preeval_mirror = DefinitionTable_MakeAnonymousDefinition(
    env->definition_table,
    preevalword
  );

  Word compositeword = DefinitionTable_TokToWord(env->definition_table,
    COMPOSITEWORD);
  CompositeDefinition *preevalwordefptr = malloc(sizeof(CompositeDefinition));
  preevalwordefptr->f = old_preeval_mirror;
  preevalwordefptr->g = wordlistpreeval;
  Definition preevalworddef;
  preevalworddef.type = compositeword;
  preevalworddef.value.pointer = preevalwordefptr;
  DefinitionTable_SetDefinition(env->definition_table, preevalword,
    preevalworddef);
}

WordArrayList ExtractWordlistFromValue (Environment *env, Word *value_ptr) {
  unsigned long *header_ptr_i = (unsigned long *)value_ptr;
  unsigned long *size_ptr = header_ptr_i - 1;
  unsigned long *count_ptr = size_ptr - 1;
  unsigned long count = *count_ptr;
  WordArrayList result;
  result.size = count;
  result.data = malloc(sizeof(Word) * count);
  unsigned long iterator = 0;
  Word listdelimiterword = DefinitionTable_TokToWord(env->definition_table,
    LISTDELIMITERWORD);
  Word *focus;
  for (
    focus = ((Word*)count_ptr) - 1;
    (focus->major != listdelimiterword.major ||
    focus->minor != listdelimiterword.minor) &&
    (void*)focus >= env->value_stack->data &&
    iterator < count;
    focus = (Word*)(((void*)focus) - Types_ResolveTypeSize(env,focus)) - 1
  ) {
    result.data[iterator] = *(focus - 1);
    iterator++;
  }
  return result;
}
