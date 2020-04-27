#if INTERFACE
#include <stdio.h>
#include <readline/readline.h>
#include "../environment.h"
#endif

#include "io.h"

void _println (Environment *env) {
  if (env->value_stack->size > 0) {
    ArbitraryValue *val = pop_stack(env->value_stack);  
    if ((val->type == stringtype) | (val->type == quotetype)) {
      printf("%s\n", val->value);
    } else {
      printf("%d\n", *(long*)val->value);
    }
    cleanup_value(val);
  }
}

void _print (Environment *env) {
  if (env->value_stack->size > 0) {
    ArbitraryValue *val = pop_stack(env->value_stack);  
    if ((val->type == stringtype) | (val->type == quotetype)) {
      printf("%s", val->value);
    } else {
      printf("%d", *(long*)val->value);
    }
    cleanup_value(val);
  }
}

void _readline (Environment *env) {
  ArbitraryValue *val = malloc(sizeof(ArbitraryValue));
  val->dynamic = 1;
  val->type = stringtype;
  val->value = readline(NULL);
  push_stack(env->value_stack, val);
}

void register_io_words (Environment *env) {
  set_value_dictionary(env->dictionaries[primarydictionary], "println", _println);
  set_value_dictionary(env->dictionaries[primarydictionary], "print", _print);
  set_value_dictionary(env->dictionaries[primarydictionary], "readln", _readline);
}