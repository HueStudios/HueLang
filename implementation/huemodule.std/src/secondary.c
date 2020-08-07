#if INTERFACE

#include "environment.h"
#include "typeannotations.h"
#include "wordlist.h"
#include "list.h"

#define DEFINESECONDARYWORD "#:"
#define SECONDARYDEFINITIONWORD "secondary"

typedef WordArrayList SecondaryDefinition;

#endif

#include "secondary.h"

void __secondary(Environment *env) {
  if (env->execution_stack == NULL) {
    return;
  }
  Word word = Environment_PopExecution(env);
  // Obtain the definition of the word.
  Definition worddef = DefinitionTable_GetDefinition(env->definition_table,
    word);
  SecondaryDefinition *wordlist = worddef.value.pointer;
  for (unsigned int i = 0; i < wordlist->size; i++) {
    Environment_PushExecution(env,wordlist->data[i]);
  }
}

void __define_secondary (Environment *env) {
  Word *name_header = ValueStack_Pop(env->value_stack, env);
  Word to_define = *(name_header - 1);
  Word *annotation_header = ValueStack_Pop(env->value_stack, env);
  WordArrayList *annotation = ExtractWordlistFromValue(env, annotation_header);
  Word *production_header = ValueStack_Pop(env->value_stack, env);
  WordArrayList *production = ExtractWordlistFromValue(env, production_header);
  Word *definition_header = ValueStack_Pop(env->value_stack,env);
  WordArrayList *definition = ExtractWordlistFromValue(env, definition_header);
  char *to_define_name = DefinitionTable_GetName(env->definition_table,
    to_define);
  Word intermediate = DefinitionTable_CreateRandomUndefined(
    env->definition_table,
    to_define_name
  );
  Word secondarydefinitionword = DefinitionTable_TokToWord(
    env->definition_table,
    SECONDARYDEFINITIONWORD
  );
  Definition defstruct;
  defstruct.value.pointer = definition;
  defstruct.type = secondarydefinitionword;
  DefinitionTable_SetDefinition(env->definition_table, intermediate,
    defstruct);
  TypeAnnotations_MakeDefinition(env, to_define);
  TypeAnnotations_AddEntry(env, to_define, intermediate, annotation,
    production);
}

void Secondary_Initialize(Environment *env) {
  Word secondaryword = DefinitionTable_TokToWord(env->definition_table,
    SECONDARYDEFINITIONWORD);
  Environment_AddPrimaryDefinition(env, secondaryword, &__secondary);


  Word defsec_intermediate = DefinitionTable_CreateRandomUndefined(
    env->definition_table,
    DEFINESECONDARYWORD
  );
  Environment_AddPrimaryDefinition(env, defsec_intermediate,
    &__define_secondary);
  Word listword = DefinitionTable_TokToWord(
    env->definition_table,
    LISTWORD
  );
  Word wordword = DefinitionTable_TokToWord(
    env->definition_table,
    WORDWORD
  );

  WordArrayList *defsec_annotation = malloc(sizeof(WordArrayList));
  defsec_annotation->size = 4;
  defsec_annotation->data = malloc(sizeof(Word)*3);
  defsec_annotation->data[0] = wordword;
  defsec_annotation->data[1] = listword;
  defsec_annotation->data[2] = listword;
  defsec_annotation->data[3] = listword;

  WordArrayList *defsec_production = malloc(sizeof(WordArrayList));
  defsec_production->size = 0;

  Word defsecword = DefinitionTable_TokToWord(env->definition_table,
    DEFINESECONDARYWORD);
  TypeAnnotations_MakeDefinition(env, defsecword);

  TypeAnnotations_AddEntry(env, defsecword, defsec_intermediate,
    defsec_annotation, defsec_production);
}
