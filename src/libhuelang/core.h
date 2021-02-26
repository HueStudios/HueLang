#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <iostream>

#define WORDWORD "word"
#define STOPCOMPWORD "__stopcomposition?"
#define WORDCOMPREHENSIONWORD "__wordcomprehension"
#define UNDEFINEDFINALWORD "__undefinedfinal"
#define INTSTATEFLAG "__intstate"
#define COMPOSITEWORD "composite"
#define INACTIONWORD "inaction"
#define USEWORD "use"

#include "word.h"
#include "value.h"
#include "definition.h"
#include <dlfcn.h>

namespace huelang
{
    class WordValue: public Value {
        public: 
            Word contained;
            ValueType getType(Environment& env);
            WordValue(Word to_contain);
    };

    struct CompositeDefinition
    {
        Word f;
        Word g;
    };

    void __primary(Environment& env);
    void __undefined_final(Environment& env);
    void __instate_flag(Environment& env);
    void __word_comprehension(Environment& env);
    void __composite(Environment& env);
    void __inaction(Environment& env);
    void InitializeCore(Environment& env);
} // namespace huelang

#endif