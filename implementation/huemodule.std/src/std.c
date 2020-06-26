#if INTERFACE

#include "environment.h"

#endif

#include "std.h"

void __hello(Environment *env) {
  printf("Hello from std lib :D\n");
}

void HueModule_Initialize (Environment *env) {
  Word helloword = DefinitionTable_TokToWord(env->definition_table,
    "hello");
  Environment_AddPrimaryDefinition(env, helloword, &__hello);
}
