#if INTERFACE
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "environment.h"
#include "tokenize.h"
#include "crudestream.h"
#endif

#include "interpreter.h"

void Interpreter_Interpret(Environment *env, int (*read_func)(char *, int)) {
  while (1) {
    unsigned int len = 0;
    char *tok = Tokens_ReadTokFromStream(read_func,&len);
    if (len == 0) {
      break;
    }
    Interpreter_ExecuteToken(env, tok);
    free(tok);
  }
}

void Interpreter_ExecuteToken (Environment *env, char *token) {
  Word tokenword = DefinitionTable_TokToWord(env->definition_table, token);
  Environment_PushExecution(env, tokenword);
  Environment_Run(env);
}

void Interpreter_InterpretFile (Environment *env, char *path) {
  int fd = open(path, O_RDONLY);
  int read_func(char *buffer, int len) {
    return read(fd, buffer, len);
  }
  Interpreter_Interpret(env, &read_func);
  close(fd);
}
