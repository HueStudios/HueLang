#include "std.h"

extern "C" void ModuleEntry(Environment *env) {
    cout << "Std loaded owe" << endl;
    Word helloword = env->definitionTable.TokToWord("hello");
    env->AddPrimaryDefinition(helloword, &__hello);
}


void __hello(Environment& env) {
    cout << "This word was added by the STD lib" << endl;
}