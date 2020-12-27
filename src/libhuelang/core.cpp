#include "core.h"

using namespace std;

namespace huelang
{
    ValueType WordValue::getType(Environment& env) {
        DefinitionTable table = env.definitionTable;
        Word wordword = table.TokToWord(WORDWORD);
        return ValueType{NULL, wordword, false};
    }

    WordValue::WordValue(Word to_contain) {
        contained = to_contain;
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

    void __undefined_final(Environment& env) {
        if (env.executionStack.empty()) return;
        Word word = env.executionStack.top();
        env.executionStack.pop();
        string name = env.definitionTable.GetName(word);
        cout << "Undefined word >" << name << "<" << endl;
    }

    void __instate_flag(Environment& env) {
        if (env.executionStack.empty()) return;
        Word word = env.executionStack.top();
        env.executionStack.pop();
        string name = env.definitionTable.GetName(word);
        cout << "Internal state" << endl;
    }

    void __word_comprehension(Environment& env) {
        if (env.executionStack.empty()) return;
        Word word = env.executionStack.top();
        string name = env.definitionTable.GetName(word);
        if (name[0] == ':') {
            string word_lit_name = name.substr(1);
            Word word_lit = env.definitionTable.TokToWord(word_lit_name);
            env.valueStack.push(WordValue{word_lit});

            Word stopCompWord = env.definitionTable.TokToWord(STOPCOMPWORD);
            Word flagType = env.definitionTable.TokToWord(INTSTATEFLAG);
            env.definitionTable[stopCompWord].type = flagType;
            env.definitionTable[stopCompWord].value.number = 1;
            env.executionStack.pop();
        }
    }
} // namespace huelang
