#if INTERFACE
#include <stdio.h>
#include "../environment.h"
#endif

#include "arithmetic.h"

void _add(Environment *env) {
  if (env->value_stack->size >= 2) {
    ArbitraryValue *a = pop_stack(env->value_stack);
    ArbitraryValue *b = pop_stack(env->value_stack);
    if ((a->type == longtype) & (b->type == longtype)) {
      long *result = malloc(sizeof(long));
      *result = (*(long*)a->value) + (*(long*)b->value);
      ArbitraryValue *result_val = malloc(sizeof(ArbitraryValue));
      result_val->type = longtype;
      result_val->dynamic = 1;
      result_val->value = result;
      push_stack(env->value_stack, result_val);
      cleanup_value(a);
      cleanup_value(b);
    } else {
      push_stack(env->value_stack, b);
      push_stack(env->value_stack, a);
    }
  }
}

void register_arithmetic_words (Environment *env) {
  set_value_dictionary(env->dictionaries[primarydictionary], "+", _add);
}

/*void _println (Environment *env) {
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
}*/