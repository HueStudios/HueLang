#include "core.h"

namespace huelang
{
    ValueType WordValue::getType(Environment& env) {
        DefinitionTable table = env.definitionTable;
        Word wordword = table.TokToWord(WORDWORD);
        return ValueType{NULL, wordword, false};
    }

    void __primary(Environment& env) {
        if (env.executionStack.empty()) return;
        Word word = env.executionStack.top();
        env.executionStack.pop();
        Definition definition = env.definitionTable[word];
        void (*defhandler)(Environment&) 
                = definition.value.handler;
        defhandler(env);
    }
} // namespace huelang
