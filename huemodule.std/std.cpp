#include "std.h"

extern "C" void ModuleEntry(Environment *env) {
    cout << "Std library loaded" << endl;
    Word helloword = env->definitionTable.TokToWord("hello");
    env->AddPrimaryDefinition(helloword, &__hello);
}


void __hello(Environment& env) {
    cout << "This word was added by the STD lib" << endl;
    for (int i = 0; i < env.valueStack.size(); i++) {
        cout << i << " : " << env.definitionTable.GetName(((WordValue*)env.valueStack.c[i])->contained) << endl;
    }
}
