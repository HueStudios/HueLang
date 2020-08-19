#if INTERFACE

#include "environment.h"
#include "list.h"
#include "secondary.h"
#include "wordlist.h"
#include "typeannotations.h"

#endif

#include "std.h"

void __hello(Environment *env) {
  printf("Hello from std lib :D\n");
}

void HueModule_Initialize (Environment *env) {
  Word helloword = DefinitionTable_TokToWord(env->definition_table,
    "hello");
  Environment_AddPrimaryDefinition(env, helloword, &__hello);
  Lists_Initialize(env);
  WordLists_Initialize(env);
  TypeAnnotations_Initialize(env);
  Secondary_Initialize(env);
}
