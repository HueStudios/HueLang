#include "definition.h"

namespace huelang {

    unsigned long long HashString(const char *str) {
        unsigned long long hash = 5381;
        int c;

        while ((c = *str++))
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash;
    }

    Word DefinitionTable::TokToWord(string token) {
        Word result;
        unsigned long long hash = HashString(token.c_str());
        unsigned int bucket_index = hash & DEF_BUCKET_COUNT;
        result.major = bucket_index;
        for (unsigned int i = 0; i < buckets[bucket_index].size(); i++) {
            if (buckets[bucket_index][i].name == token) {
                result.minor = i;
                return result;
            }
        }

        if (token != UNDEFINEDWORD) {
            Definition defaultdef;
            defaultdef.value.number = 0;
            defaultdef.type = TokToWord(UNDEFINEDWORD);
            result.minor = buckets[bucket_index].size();
            buckets[bucket_index].push_back(DefinitionTableEntry{
                token,
                defaultdef
            });
        }
        
        return result;
    }

    Definition& DefinitionTable::operator[](Word word) {
        return buckets[word.major][word.minor].definition;
    }

};