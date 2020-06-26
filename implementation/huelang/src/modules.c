#if INTERFACE

#include "environment.h"
#include <string.h>
#include <signal.h>
#include "interpreter.h"

#define INCLUDEWORD "#include"
#define INCLUSIONWDWORD "__inclusionwd"

#endif

#include "modules.h"

char *Modules_PathWithoutFilename (char *path) {
  unsigned int path_len = strlen(path) + 1;
  char *new_path = malloc(path_len * sizeof(char));
  unsigned int final_separator = 0;
  unsigned char found_separator = 0;
  for(unsigned int i = 0; i < path_len; i++) {
    new_path[i] = path[i];
    if (path[i] == '/') {
      final_separator = i;
      found_separator = 1;
    }
  }
  new_path[final_separator + found_separator] = '\0';
  unsigned int final_len = final_separator + 1 + found_separator;
  new_path = realloc(new_path, final_len);
  return new_path;
}

char *Modules_AppendPath(char *a, char *b) {
  unsigned int lena = strlen(a);
  unsigned int lenb = strlen(b);
  unsigned int newlen = lena + lenb + 1;
  char *new_path = malloc(sizeof(char)*newlen);
  memcpy(new_path,a,lena);
  for (unsigned int i = 0; i < lenb; i++) {
    new_path[i+lena] = b[i];
  }
  new_path[newlen-1] = '\0';
  return new_path;
}

void __include (Environment *env) {
  if (env->value_stack->pointer == 0) {
    printf("exited\n");
    return;
  }
  Word *wordvalue = ValueStack_Pop(env->value_stack,env);
  Word word = *(wordvalue - 1);
  char *name = DefinitionTable_GetName(env->definition_table, word);

  Word inclusionwdword = DefinitionTable_TokToWord(
    env->definition_table,
    INCLUSIONWDWORD);

  Definition inclusionwddef = DefinitionTable_GetDefinition(
    env->definition_table,
    inclusionwdword
  );

  char *prev_inclusionwd = inclusionwddef.value.pointer;

  char *inclusionpath = Modules_PathWithoutFilename(name);
  if (strlen(inclusionpath) == 0) {
    char *new_name = Modules_AppendPath(prev_inclusionwd,name);
    Interpreter_InterpretFile(env, new_name);
    free(new_name);
  } else {
    if (inclusionpath[0] == '/' || strlen(prev_inclusionwd) == 0) {
      inclusionwddef.value.pointer = inclusionpath;
      DefinitionTable_SetDefinition(
        env->definition_table,
        inclusionwdword,
        inclusionwddef
      );
      Interpreter_InterpretFile(env, name);
    } else {
      char *new_name = Modules_AppendPath(prev_inclusionwd,name);
      char *new_path = Modules_AppendPath(prev_inclusionwd,inclusionpath);
      inclusionwddef.value.pointer = new_path;
      DefinitionTable_SetDefinition(
        env->definition_table,
        inclusionwdword,
        inclusionwddef
      );
      Interpreter_InterpretFile(env, new_name);
      free(new_name);
      free(new_path);
    }
    inclusionwddef.value.pointer = prev_inclusionwd;
    DefinitionTable_SetDefinition(
      env->definition_table,
      inclusionwdword,
      inclusionwddef
    );
  }
  free(inclusionpath);
}

void Modules_Initialize (Environment *env) {
  Word includeword = DefinitionTable_TokToWord(env->definition_table,
    INCLUDEWORD);
  Environment_AddPrimaryDefinition(env, includeword, &__include);

  Word inclusionwdword = DefinitionTable_TokToWord(env->definition_table,
    INCLUSIONWDWORD);
  Word flagtype = DefinitionTable_TokToWord(env->definition_table,
    INTSTATEFLAG);
  Definition inclusionwddef;
  inclusionwddef.type = flagtype;
  inclusionwddef.value.pointer = "";
  DefinitionTable_SetDefinition(env->definition_table, inclusionwdword,
    inclusionwddef);
}
