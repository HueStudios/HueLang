#if INTERFACE
#include <stdio.h>
#include "../environment.h"
#endif

#include "io.h"

void _println (Environment *env) {
  if (env->value_stack->size > 0) {
    ArbitraryValue *val = pop_stack(env->value_stack);  
    if ((val->type == stringtype) || (val->type == quotetype)) {
      printf("%s\n", val->value);
    } else {
      printf("%d\n", *(int*)val->value);
    }
    if (val->dynamic) {
      free(val->value);
      free(val);
    }
  }
}

void register_io_words (Environment *env) {
  set_value_dictionary(env->dictionaries[primarydictionary], "println", _println);
}