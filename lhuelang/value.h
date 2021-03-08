#ifndef VALUE_H
#define VALUE_H

#include <vector>
#include "word.h"

using namespace std;

namespace huelang
{

    class Environment;

    struct ValueType {
        vector<ValueType> *compound;
        Word atom;
        bool isCompound;
    };
    bool operator==(const ValueType& a, const ValueType& b);

    class Value {
        public:
            ValueType getType(Environment& env);
    };
};

#include "environment.h"

#endif