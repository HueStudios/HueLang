#include "core.h"

namespace huelang
{
    ValueType WordValue::getType(const Environment& env) {
        DefinitionTable table = env.definitionTable;
        Word wordword = table.TokToWord(WORDWORD);
        return ValueType{NULL, wordword, false};
    }
} // namespace huelang
