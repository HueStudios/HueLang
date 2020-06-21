#if INTERFACE
#include <stdio.h>
#include "environment.h"
#include "tokenize.h"
#include <signal.h>
#endif

#include "repl.h"


void __test(Environment *env) {
  printf("HELLO WORLD\n");
}

void Repl_Start() {
  Environment *env = Environment_Create();
  Word testword = DefinitionTable_TokToWord(env->definition_table, "test");
  Environment_AddPrimaryDefinition(env, testword, &__test);
  while (1) {
    unsigned int len = 0;
    char *tok = Tokens_ReadTokFromFDesc(0,&len);
    if (len == 0) {
      break;
    }
    Repl_ExecuteToken(env, tok);
  }
}

void Repl_ExecuteToken (Environment *env, char *token) {
  Word tokenword = DefinitionTable_TokToWord(env->definition_table, token);
  Environment_PushExecution(env, tokenword);
  Environment_Run(env);
}