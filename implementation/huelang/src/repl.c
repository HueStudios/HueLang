#if INTERFACE
#include <stdio.h>
#include "environment.h"
#include "interpreter.h"
#include <readline/readline.h>
#include <readline/history.h>
#include "logo.h"
#include "crudestream.h"
#endif

#include "repl.h"

void Repl_Start() {
  printf("%s  HueLang version pre-alpha\n\n",huelang_txt);
  Environment *env = Environment_Create();
  rl_bind_key('\t', rl_insert);
  char* buf;

  CrudeStream stream;

  int read_func(char *buffer, int len) {
    return CrudeStream_Read(&stream, buffer, len);
  }

  while ((buf = readline(">> ")) != NULL) {
    int len = strlen(buf);
    if (len > 0) {
      add_history(buf);
    }

    stream.data = buf;
    stream.length = len;
    stream.index = 0;
    Interpreter_Interpret(env, &read_func);


    // readline malloc's a new buffer every time.
    free(buf);
  }
  printf("\n");
}