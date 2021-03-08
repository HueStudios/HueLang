#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <stack>
#include "word.h"
#include "definition.h"
#include "value.h"

#define PRIMARYDEFINITIONWORD "primary"
#define PREEVALWORD "preeval"

#include <stdio.h>
#include <iostream>

using namespace std;

namespace huelang {

    class ExecutionStack : public stack<Word> {
    public:
        using stack<Word>::c;
    };

    class ValueStack : public stack<Value*> {
    public:
        using stack<Value*>::c;
    };

    class Environment {
        public:
            void Evaluate();
            void Run();
            void AddPrimaryDefinition(Word word, void (*handler)(Environment&));
            DefinitionTable definitionTable;
            ExecutionStack executionStack;
            ValueStack valueStack;
        private:
            
    };

};

#endif