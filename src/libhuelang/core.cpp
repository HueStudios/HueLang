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

    void __composite(Environment& env) {
        if (env.executionStack.empty()) return;
        Word word = env.executionStack.top();
        env.executionStack.pop();

        CompositeDefinition *compdef = 
            (CompositeDefinition*)env.definitionTable[word].value.pointer;
        
        Word stopCompWord = env.definitionTable.TokToWord(STOPCOMPWORD);
        Word flagType = env.definitionTable.TokToWord(INTSTATEFLAG);

        Definition& stcompdef = env.definitionTable[stopCompWord];
        stcompdef.type = flagType;
        stcompdef.value.number = 0;

        env.executionStack.push(compdef->f);
        env.Evaluate();

        if (stcompdef.value.number != 1) {
            env.executionStack.push(compdef->f);
            env.Evaluate();
        }
    }

    void __inaction(Environment& env) {
        if (env.executionStack.empty()) return;
        env.executionStack.pop();        
    }

    void InititializeCore(Environment& env) {
        Word wordword = env.definitionTable.TokToWord(WORDWORD);

        Word primaryword = env.definitionTable.TokToWord(PRIMARYDEFINITIONWORD);
        env.AddPrimaryDefinition(primaryword, &__primary);

        Word intflagword = env.definitionTable.TokToWord(INTSTATEFLAG);
        env.AddPrimaryDefinition(intflagword, &__instate_flag);

        Word wordcompword 
            = env.definitionTable.TokToWord(WORDCOMPREHENSIONWORD);
        env.AddPrimaryDefinition(wordcompword, &__word_comprehension);

        Word undefinedfinalword
            = env.definitionTable.TokToWord(UNDEFINEDFINALWORD);
        env.AddPrimaryDefinition(undefinedfinalword, &__undefined_final);

        Word compositeword = env.definitionTable.TokToWord(COMPOSITEWORD);
        env.AddPrimaryDefinition(compositeword, &__composite);

        Word inactionword = env.definitionTable.TokToWord(INACTIONWORD);
        env.AddPrimaryDefinition(inactionword, &__inaction);

        Word preevalword = env.definitionTable.TokToWord(PREEVALWORD);
        Definition& preevaldefinition = env.definitionTable[preevalword];
        preevaldefinition.type = inactionword;
        preevaldefinition.value.number = 0;

        Word undefinedword = env.definitionTable.TokToWord(UNDEFINEDWORD);
        CompositeDefinition *undefinedworddefptr = new CompositeDefinition();
        undefinedworddefptr->f = wordcompword;
        undefinedworddefptr->g = undefinedfinalword;
        Definition& undefineddefinition 
            = env.definitionTable[undefinedword];
        undefineddefinition.type = compositeword;
        undefineddefinition.value.pointer = undefinedworddefptr;
    }
} // namespace huelang
