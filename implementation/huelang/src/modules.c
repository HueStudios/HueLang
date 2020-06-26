#if INTERFACE

#include "environment.h"
#include <string.h>
#include <dlfcn.h>
#include "interpreter.h"

#define MODULE_FILENAME_INITIAL "huemodule."
#define MODULE_FILE_EXTENSION ".so"

#define INCLUDEWORD "#include"
#define INCLUSIONWDWORD "__inclusionwd"

#define USEWORD "#use"

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

char *Modules_ObtainModuleFilename (char *modulename) {
  char *initial_section = MODULE_FILENAME_INITIAL;
  char *extension = MODULE_FILE_EXTENSION;
  int initial_section_len = strlen(initial_section);
  int modulename_len = strlen(modulename);
  int extension_len = strlen(extension);
  int final_len = initial_section_len + modulename_len + extension_len + 1;
  char *result = malloc(sizeof(char) * final_len);
  result[0] = '\0';
  strcat(result,initial_section);
  strcat(result,modulename);
  strcat(result,extension);
  return result;
}

void __use(Environment *env) {
  if (env->value_stack->pointer == 0) {
    return;
  }

  Word *wordvalue = ValueStack_Pop(env->value_stack,env);
  Word word = *(wordvalue - 1);
  char *name = DefinitionTable_GetName(env->definition_table, word);

  char *module_filename = Modules_ObtainModuleFilename(name);

  void *handle = dlopen(module_filename, RTLD_LAZY);

  if (handle) {
    dlerror();

    void (*module_init)(Environment *) = dlsym(handle,
      "HueModule_Initialize");

    char *error = dlerror();

    if (error != NULL) {
      fprintf(stderr, "%s\n", error);
    } else {
      module_init(env);
    }
  } else {
    fprintf(stderr, "Unable to load >%s<\n", module_filename);
  }
  free(module_filename);
}

void Modules_Initialize (Environment *env) {
  Word includeword = DefinitionTable_TokToWord(env->definition_table,
    INCLUDEWORD);
  Environment_AddPrimaryDefinition(env, includeword, &__include);

  Word useword = DefinitionTable_TokToWord(env->definition_table,
    USEWORD);
  Environment_AddPrimaryDefinition(env, useword, &__use);

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
