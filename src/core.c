#if INTERFACE
#include "environment.h"
#endif

#include "core.h"

void _def(Environment *env) {
    push_state_environment(env, compilestate);
}

void _def_compile(Environment *env) {
    preppend_linked(env->word_buffer, "def");
    push_state_environment(env, hardcompilestate);
}

void _end_compile(Environment *env) {
    pop_state_environment(env);
    if (get_state_environment(env) != compilestate) {
        set_value_dictionary(env->dictionaries[secondarydictionary], env->word_name, 
            env->word_buffer);
        env->word_name = NULL;
        env->word_buffer = create_linked();
    }
}

void register_core(Environment *env) {
    set_value_dictionary(env->dictionaries[primarydictionary], "def", _def);
    set_value_dictionary(env->dictionaries[compiledictionary], "def", _def_compile);
    set_value_dictionary(env->dictionaries[compiledictionary], "end", _end_compile);
}