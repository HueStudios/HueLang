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

    class Value {
        public:
            ValueType getType(Environment& env);
    };
};

#include "environment.h"

#endif