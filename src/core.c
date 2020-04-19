#if INTERFACE
#include "environment.h"
#endif

#include "core.h"

void _def(Environment *env) {
  //printf("Entered compile\n");
  push_state_environment(env, compilestate);
}

void _def_compile(Environment *env) {
  //printf("Entered hard compile\n");
  ArbitraryValue *def_val = malloc(sizeof(ArbitraryValue));
  def_val->value = DEF;
  def_val->dynamic = 0;
  def_val->type = symboltype;
  preppend_linked(env->word_buffer, def_val);
  push_state_environment(env, hardcompilestate);
}

void _end_compile(Environment *env) {
  //printf("Exited compile\n");
  pop_state_environment(env);
  if (get_state_environment(env) != compilestate) {
    set_value_dictionary(env->dictionaries[secondarydictionary], env->word_name, 
      env->word_buffer);
    //printf("Compiled word named %s\n", env->word_name);
    env->word_name = NULL;
    env->word_buffer = create_linked();
  }
}

void _use(Environment *env) {
  if (env->value_stack->size > 0) {
    ArbitraryValue *val = pop_stack(env->value_stack);
    if ((val->type == stringtype) | (val->type == quotetype)) {
      if (contains_key_dictionary(env->vocabularies, 
      val->value)) {
        void (*vocab_loader)(Environment*) 
          = get_value_dictionary(env->vocabularies, val->value);
        vocab_loader(env);
      }
    }
  }
}

void register_core_words(Environment *env) {
  set_value_dictionary(env->dictionaries[primarydictionary], "use", _use);
  set_value_dictionary(env->dictionaries[primarydictionary], DEF, _def);
  set_value_dictionary(env->dictionaries[compiledictionary], DEF, _def_compile);
  set_value_dictionary(env->dictionaries[compiledictionary], END, _end_compile);
}