#if INTERFACE

typedef struct Environment Environment;

#define WORDWORD "word"
#define STOPCOMPWORD "__stopcomposition?"
#define WORDCOMPREHENSIONWORD "__wordcomprehension"
#define UNDEFINEDFINALWORD "__undefinedfinal"
#define INTSTATEFLAG "__flag"
#define COMPOSITEWORD "composite"
#include <stdio.h>
#include <signal.h>
#include "environment.h"
#include "type.h"
#include "values.h"

typedef struct CompositeDefinition
{
  Word f;
  Word g;
} CompositeDefinition;

#endif

#include "core.h"

void __primary(Environment *env) {
  if (env->execution_stack == NULL) {
    return;
  }
  Word word = Environment_PopExecution(env);
  // Obtain the definition of the word.
  Definition worddef = DefinitionTable_GetDefinition(env->definition_table,
    word);
  void (*defhandler)(Environment*) = worddef.value.pointer;
  defhandler(env);
}

void __undefined_final(Environment *env) {
  if (env->execution_stack == NULL) {
    return;
  }
  Word word = Environment_PopExecution(env);
  char *name = DefinitionTable_GetName(env->definition_table, word);
  printf("Undefined word >%s<\n", name);
}

void __intstate_flag(Environment *env) {
  if (env->execution_stack == NULL) {
    return;
  }
  Environment_PopExecution(env);
  printf("Internal state flag\n");
}

void __atomic(Environment *env) {
  if (env->execution_stack == NULL) {
    return;
  }
  Word word = Environment_PopExecution(env);
  unsigned int size = Types_ResolveTypeSize(env, &word);
  printf("Atomic type of size %u\n", size);
}

void __compound(Environment *env) {
  if (env->execution_stack == NULL) {
    return;
  }
  Environment_PopExecution(env);
  printf("Compound type\n");
}

void __word_comprehension(Environment *env) {
  if (env->execution_stack == NULL) {
    return;
  }
  Word word = Environment_PeekExecution(env);
  char *name = DefinitionTable_GetName(env->definition_table, word);
  if (name[0] == ':') {
    Word wordword = DefinitionTable_TokToWord(env->definition_table,
      WORDWORD);
    char *word_lit_name = name + 1;
    Word word_lit = DefinitionTable_TokToWord(env->definition_table,
      word_lit_name);
    Word *salloc = ValueStack_PushAlloc(env->value_stack, sizeof(Word)*2);
    salloc[0] = word_lit;
    salloc[1] = wordword;
    Word stopCompFlagWord = DefinitionTable_TokToWord(env->definition_table,
      STOPCOMPWORD);
    Word flagtype = DefinitionTable_TokToWord(env->definition_table,
      INTSTATEFLAG);
    Definition stopCompDef;
    stopCompDef.type = flagtype;
    stopCompDef.value.number = 1;
    DefinitionTable_SetDefinition(env->definition_table, stopCompFlagWord,
      stopCompDef);
    Environment_PopExecution(env);
  }
}

void __composite(Environment *env) {
  if (env->execution_stack == NULL) {
    return;
  }
  Word word = Environment_PopExecution(env);
  // Obtain the definition of the word.
  Definition worddef = DefinitionTable_GetDefinition(env->definition_table,
    word);
  CompositeDefinition *compdef = worddef.value.pointer;

  Word stopCompFlagWord = DefinitionTable_TokToWord(env->definition_table,
    STOPCOMPWORD);
  Word flagtype = DefinitionTable_TokToWord(env->definition_table,
    INTSTATEFLAG);

  volatile Definition stopCompDef;
  stopCompDef.type = flagtype;
  stopCompDef.value.number = 0;
  DefinitionTable_SetDefinition(env->definition_table, stopCompFlagWord,
    stopCompDef);

  Environment_PushExecution(env, compdef->f);
  Environment_Evaluate(env);

  stopCompDef = DefinitionTable_GetDefinition(env->definition_table,
    stopCompFlagWord);

  if (stopCompDef.value.number != 1) {
    Environment_PushExecution(env, compdef->g);
    Environment_Evaluate(env);
  }
}

void Core_Initialize(Environment *env) {
  // This is the best piece of code I've written in my life.
  Word wordword = DefinitionTable_TokToWord(env->definition_table, WORDWORD);
  Types_RegisterAtomicType(env, wordword, sizeof(Word));

  Word atomictypeword = DefinitionTable_TokToWord(env->definition_table,
    ATOMTYPEDEFWORD);
  Environment_AddPrimaryDefinition(env, atomictypeword, &__atomic);

  Word compoundtypeword = DefinitionTable_TokToWord(env->definition_table,
    COMPOUNDTYPEDEFWORD);
  Environment_AddPrimaryDefinition(env, compoundtypeword, &__compound);

  Word primaryword = DefinitionTable_TokToWord(env->definition_table,
    PRIMARYDEFINITIONWORD);
  Environment_AddPrimaryDefinition(env, primaryword, &__primary);

  Word intflagword = DefinitionTable_TokToWord(env->definition_table,
    INTSTATEFLAG);
  Environment_AddPrimaryDefinition(env, intflagword, &__intstate_flag);

  Word wordcompword = DefinitionTable_TokToWord(env->definition_table,
    WORDCOMPREHENSIONWORD);
  Environment_AddPrimaryDefinition(env, wordcompword, &__word_comprehension);

  Word undefinedfinalword = DefinitionTable_TokToWord(env->definition_table,
    UNDEFINEDFINALWORD);
  Environment_AddPrimaryDefinition(env, undefinedfinalword, &__undefined_final);

  Word compositeword = DefinitionTable_TokToWord(env->definition_table,
    COMPOSITEWORD);
  Environment_AddPrimaryDefinition(env, compositeword, &__composite);

  Word undefinedword = DefinitionTable_TokToWord(env->definition_table,
    UNDEFINEDWORD);
  CompositeDefinition *undefinedwordefptr = malloc(sizeof(CompositeDefinition));
  undefinedwordefptr->f = wordcompword;
  undefinedwordefptr->g = undefinedfinalword;
  Definition undefinedworddef;
  undefinedworddef.type = compositeword;
  undefinedworddef.value.pointer = undefinedwordefptr;
  DefinitionTable_SetDefinition(env->definition_table, undefinedword,
    undefinedworddef);
}
