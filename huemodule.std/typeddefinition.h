#ifndef TYPEDDEFINITION_H
#define TYPEDDEFINITION_H

#include <vector>

#include <lhuelang/environment.h>
#include <lhuelang/value.h>
#include <lhuelang/word.h>
#include <lhuelang/definition.h>

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