#ifndef VALUE_H
#define VALUE_H

#include <vector>
#include "word.h"
#include "environment.h"

using namespace std;

namespace huelang
{

    struct ValueType {
        vector<ValueType> *compound;
        Word atom;
        bool isCompound;
    };

    class Value {
        public:
            virtual ValueType getType(const Environment& env) = 0;
    };
};

#endif