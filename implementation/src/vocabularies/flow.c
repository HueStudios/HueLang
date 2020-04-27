#if INTERFACE
#include <stdio.h>
#include <string.h>
#include "../environment.h"
#endif

#include "flow.h"

void _if (Environment *env) {
  if (env->value_stack->size >= 3) {
    ArbitraryValue *false_sel = pop_stack(env->value_stack);
    ArbitraryValue *true_sel = pop_stack(env->value_stack);
    ArbitraryValue *det = pop_stack(env->value_stack);
    if (((true_sel->type == stringtype) | (true_sel->type == quotetype)) & 
      ((false_sel->type == stringtype) | (false_sel->type == quotetype)) &
      (det->type == longtype)) {
      ArbitraryValue *selected;
      if (*((long*)det->value)) {
        selected = true_sel;
      } else {
        selected = false_sel;
      }
      ArbitraryValue *to_call = malloc(sizeof(ArbitraryValue));
      to_call->type = symboltype;
      to_call->dynamic = 1;
      to_call->value = malloc(sizeof(char) * (strlen(selected->value) + 1));
      push_stack(env->execution_stack, to_call);
      strcpy(to_call->value, selected->value);
      cleanup_value(true_sel);
      cleanup_value(false_sel);
      cleanup_value(det);
    } else {
      push_stack(env->value_stack, det);
      push_stack(env->value_stack, true_sel);
      push_stack(env->value_stack, false_sel);
    }
  }
}

void _when (Environment *env) {
  if (env->value_stack->size >= 2) {
    ArbitraryValue *true_sel = pop_stack(env->value_stack);
    ArbitraryValue *det = pop_stack(env->value_stack);
    if (((true_sel->type == stringtype) | (true_sel->type == quotetype)) &
      (det->type == longtype)) {
      ArbitraryValue *selected;
      if (*((long*)det->value)) {
        selected = true_sel;
        ArbitraryValue *to_call = malloc(sizeof(ArbitraryValue));
        to_call->type = symboltype;
        to_call->dynamic = 1;
        to_call->value = malloc(sizeof(char) * (strlen(selected->value) + 1));
        strcpy(to_call->value, selected->value);
        push_stack(env->execution_stack, to_call);
      } 
      cleanup_value(true_sel);
      cleanup_value(det);
    } else {
      push_stack(env->value_stack, det);
      push_stack(env->value_stack, true_sel);
    }
  }
}

void _do (Environment *env) {
  if (env->value_stack->size >= 1) {
    ArbitraryValue *true_sel = pop_stack(env->value_stack);
    if ((true_sel->type == stringtype) | (true_sel->type == quotetype)) {
      ArbitraryValue *selected;
      selected = true_sel;
      ArbitraryValue *to_call = malloc(sizeof(ArbitraryValue));
      to_call->type = symboltype;
      to_call->dynamic = 1;
      to_call->value = malloc(sizeof(char) * (strlen(selected->value) + 1));
      strcpy(to_call->value, selected->value);
      push_stack(env->execution_stack, to_call);
      cleanup_value(true_sel);
    } else {
      push_stack(env->value_stack, true_sel);
    }
  }
}

void register_flow_words (Environment *env) {
  set_value_dictionary(env->dictionaries[primarydictionary], "if", _if);
  set_value_dictionary(env->dictionaries[primarydictionary], "when", _when);
  set_value_dictionary(env->dictionaries[primarydictionary], "do", _do);
}