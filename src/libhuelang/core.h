#ifndef CORE_H
#define CORE_H

#define WORDWORD "word"

#include "word.h"
#include "value.h"
#include "definition.h"

namespace huelang
{
    class WordValue: public Value {
        ValueType getType(Environment& env);
        Word contained;
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