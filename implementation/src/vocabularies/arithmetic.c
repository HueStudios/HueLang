#if INTERFACE
#include <stdio.h>
#include "../environment.h"
#endif

#include "arithmetic.h"

void _generic_arithmetic_word_long(Environment *env, long (*res)(long, long)) {
  if (env->value_stack->size >= 2) {
    ArbitraryValue *b = pop_stack(env->value_stack);
    ArbitraryValue *a = pop_stack(env->value_stack);
    if ((a->type == longtype) & (b->type == longtype)) {
      long *result = malloc(sizeof(long));
      *result = res((*(long*)a->value),(*(long*)b->value));
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

long add_res (long a, long b) {
  return a + b;
}

void _add(Environment *env) {
  _generic_arithmetic_word_long(env, add_res);
}

long sub_res (long a, long b) {
  return a - b;
}

void _sub(Environment *env) {
  _generic_arithmetic_word_long(env, sub_res);
}

long mul_res (long a, long b) {
  return a * b;
}

void _mul(Environment *env) {
  _generic_arithmetic_word_long(env, mul_res);
}

long div_res (long a, long b) {
  return a / b;
}

void _div(Environment *env) {
  _generic_arithmetic_word_long(env, div_res);
}

long mod_res (long a, long b) {
  return a % b;
}

void _mod(Environment *env) {
  _generic_arithmetic_word_long(env, mod_res);
}

long and_res (long a, long b) {
  return a & b;
}

void _and(Environment *env) {
  _generic_arithmetic_word_long(env, and_res);
}

long or_res (long a, long b) {
  return a | b;
}

void _or(Environment *env) {
  _generic_arithmetic_word_long(env, or_res);
}

long rshift_res (long a, long b) {
  return a >> b;
}

void _rshift(Environment *env) {
  _generic_arithmetic_word_long(env, rshift_res);
}

long lshift_res (long a, long b) {
  return a << b;
}

void _lshift(Environment *env) {
  _generic_arithmetic_word_long(env, lshift_res);
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
