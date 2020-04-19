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
    def_val->value = "def";
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

void register_core_words(Environment *env) {
    set_value_dictionary(env->dictionaries[primarydictionary], "def", _def);
    set_value_dictionary(env->dictionaries[compiledictionary], "def", _def_compile);
    set_value_dictionary(env->dictionaries[compiledictionary], "end", _end_compile);
}