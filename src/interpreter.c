#if INTERFACE
#include "tokenize.h"
#include "environment.h"
#include "core.h"
#include "vocabularies/io.h"
#include "vocabularies/arithmetic.h"
#include <sys/stat.h>
#include <fcntl.h>
#endif 

#include "interpreter.h"


Environment *get_default_environment() {
  Environment *env = create_environment();
  register_core_words(env);
  set_value_dictionary(env->vocabularies, "io", register_io_words);
  set_value_dictionary(env->vocabularies, "arithmetic", register_arithmetic_words);
  return env;
}

void repl() {
  Environment *env = get_default_environment();
  while (1) {
    unsigned int len = 0;
    char *tok = read_token_from_filedesc(0,&len);
    //printf("Read token %s \n", tok);
    if (len == 0) {
      break;
    }
    execute_in_environment(env, tok);
  }
}

void run_file (char *path) {
  Environment *env = get_default_environment();
  int fd = open(path, O_RDONLY);
  while (1) {
    unsigned int len = 0;
    char *tok = read_token_from_filedesc(fd,&len);
    if (len == 0) {
      break;
    }
    execute_in_environment(env, tok);
  }
  close(fd);
}