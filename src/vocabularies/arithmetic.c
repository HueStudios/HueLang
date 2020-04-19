#if INTERFACE
#include <stdio.h>
#include "../environment.h"
#endif

#include "arithmetic.h"

void _add(Environment *env) {
  if (env->value_stack->size >= 2) {
    ArbitraryValue *b = pop_stack(env->value_stack);
    ArbitraryValue *a = pop_stack(env->value_stack);
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

void _sub(Environment *env) {
  if (env->value_stack->size >= 2) {
    ArbitraryValue *b = pop_stack(env->value_stack);
    ArbitraryValue *a = pop_stack(env->value_stack);
    if ((a->type == longtype) & (b->type == longtype)) {
      long *result = malloc(sizeof(long));
      *result = (*(long*)a->value) - (*(long*)b->value);
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

void _mul(Environment *env) {
  if (env->value_stack->size >= 2) {
    ArbitraryValue *b = pop_stack(env->value_stack);
    ArbitraryValue *a = pop_stack(env->value_stack);
    if ((a->type == longtype) & (b->type == longtype)) {
      long *result = malloc(sizeof(long));
      *result = (*(long*)a->value) * (*(long*)b->value);
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

void _div(Environment *env) {
  if (env->value_stack->size >= 2) {
    ArbitraryValue *a = pop_stack(env->value_stack);
    ArbitraryValue *b = pop_stack(env->value_stack);
    if ((a->type == longtype) & (b->type == longtype)) {
      long *result = malloc(sizeof(long));
      *result = (*(long*)a->value) / (*(long*)b->value);
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

void _mod(Environment *env) {
  if (env->value_stack->size >= 2) {
    ArbitraryValue *b = pop_stack(env->value_stack);
    ArbitraryValue *a = pop_stack(env->value_stack);
    if ((a->type == longtype) & (b->type == longtype)) {
      long *result = malloc(sizeof(long));
      *result = (*(long*)a->value) % (*(long*)b->value);
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

void _and(Environment *env) {
  if (env->value_stack->size >= 2) {
    ArbitraryValue *b = pop_stack(env->value_stack);
    ArbitraryValue *a = pop_stack(env->value_stack);
    if ((a->type == longtype) & (b->type == longtype)) {
      long *result = malloc(sizeof(long));
      *result = (*(long*)a->value) & (*(long*)b->value);
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

void _or(Environment *env) {
  if (env->value_stack->size >= 2) {
    ArbitraryValue *b = pop_stack(env->value_stack);
    ArbitraryValue *a = pop_stack(env->value_stack);
    if ((a->type == longtype) & (b->type == longtype)) {
      long *result = malloc(sizeof(long));
      *result = (*(long*)a->value) | (*(long*)b->value);
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

void _rshift(Environment *env) {
  if (env->value_stack->size >= 2) {
    ArbitraryValue *b = pop_stack(env->value_stack);
    ArbitraryValue *a = pop_stack(env->value_stack);
    if ((a->type == longtype) & (b->type == longtype)) {
      long *result = malloc(sizeof(long));
      *result = (*(long*)a->value) >> (*(long*)b->value);
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

void _lshift(Environment *env) {
  if (env->value_stack->size >= 2) {
    ArbitraryValue *b = pop_stack(env->value_stack);
    ArbitraryValue *a = pop_stack(env->value_stack);
    if ((a->type == longtype) & (b->type == longtype)) {
      long *result = malloc(sizeof(long));
      *result = (*(long*)a->value) >> (*(long*)b->value);
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
  set_value_dictionary(env->dictionaries[primarydictionary], "-", _sub);
  set_value_dictionary(env->dictionaries[primarydictionary], "*", _mul);
  set_value_dictionary(env->dictionaries[primarydictionary], "/", _div);
  set_value_dictionary(env->dictionaries[primarydictionary], "%", _mod);
  set_value_dictionary(env->dictionaries[primarydictionary], "&", _and);
  set_value_dictionary(env->dictionaries[primarydictionary], "|", _or);
  set_value_dictionary(env->dictionaries[primarydictionary], ">>", _rshift);
  set_value_dictionary(env->dictionaries[primarydictionary], "<<", _lshift);
}
