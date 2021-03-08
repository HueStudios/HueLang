#include "typeddefinition.h"

void __typeddefinition(Environment &env) {
    if (env.executionStack.empty()) return;
    Word word = env.executionStack.top();
    env.executionStack.pop();
    Definition definition = env.definitionTable[word];
    TypedDefinition *typeddef = (TypedDefinition*)definition.value.pointer;

    TypedDefinitionEntry *best;

    for(TypedDefinitionEntry& entry: (typeddef->entries)) {
        if (best == NULL || best->inputs.size() < entry.inputs.size()) {
            if (entry.inputs.size() > env.valueStack.size()) continue;
            for (unsigned int i = 0; i < entry.inputs.size(); i++) {
                ValueType stackType = env.valueStack.c
                    [env.valueStack.c.size() - 1 - i]->getType(env);
                ValueType entryType = entry.inputs[entry.inputs.size() - 1 - i];
                if (!(stackType == entryType)) continue;
            }
            best = &entry;
        }
    }
    if (best == NULL) {
        cerr << "No matching definition found" << endl;
        return;
    }

    env.executionStack.push(best->to_call);
    
}
