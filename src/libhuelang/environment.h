#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <stack>
#include "word.h"
#include "definition.h"
#include "value.h"

#define PRIMARYDEFINITIONWORD "primary"
#define PREEVALWORD "preeval"

using namespace std;

namespace huelang {

    class Environment {
        public:
            void Evaluate();
            void Run();
            DefinitionTable definitionTable;
            stack<Word> executionStack;
            stack<Value> valueStack;
        private:
            
    };

};

#endif