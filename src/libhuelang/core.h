#ifndef CORE_H
#define CORE_H

#define WORDWORD "word"

#include "word.h"
#include "value.h"
#include "definition.h"

namespace huelang
{
    class WordValue: public Value {
        ValueType getType(const Environment& env);
        Word contained;
    };
} // namespace huelang


#endif