#ifndef DEFINITION_H
#define DEFINITION_H

#include <vector>
#include <string>
#include "word.h"

#define UNDEFINEDWORD "undefined"
#define DEF_BUCKET_COUNT 0xFFF

using namespace std;

namespace huelang {

    class Environment;

    union DefinitionValue {
        void *pointer;
        long number;
        void (*handler)(Environment&);
    };

    struct Definition {
        DefinitionValue value;
        Word type;
    };

    struct DefinitionTableEntry
    {
        string name;
        Definition definition;
    };    

    class DefinitionTable {
        private:
            vector<DefinitionTableEntry> buckets[0xFF + 1];
            unsigned long long HashString(const char *str);
        public:
            Word TokToWord(string tok);
            string GetName(Word word);
            Definition& operator[](Word word);
    };

};

#include "environment.h"

#endif