#ifndef TYPEDDEFINITION_H
#define TYPEDDEFINITION_H

#include <vector>

#include "../libhuelang/environment.h"
#include "../libhuelang/value.h"
#include "../libhuelang/word.h"
#include "../libhuelang/definition.h"

using namespace std;
using namespace huelang;

struct TypedDefinitionEntry {
    Word to_call;
    vector<ValueType> inputs;
    vector<ValueType> outputs;
};

struct TypedDefinition {
    vector<TypedDefinitionEntry> entries;
};

void __typeddefinition(Environment &env);

#endif