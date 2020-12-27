#include "environment.h"


using namespace std;


namespace huelang
{
    void Environment::Evaluate() {
        if (executionStack.empty()) return;
        Word word = executionStack.top();
        Word primarydefinitiontype 
            = definitionTable.TokToWord(PRIMARYDEFINITIONWORD);
        if (primarydefinitiontype == word) {
            Definition primarydefinition 
                = definitionTable[primarydefinitiontype];
            void (*defhandler)(Environment&) 
                = primarydefinition.value.handler;
            defhandler(*this);
        } else {
            Definition worddef = definitionTable[word];
            Word definitiontype = worddef.type;
            executionStack.push(definitiontype);
            Evaluate();
        }
    }

    void Environment::Run() {
        while (!executionStack.empty()) {
            Word preevalword = definitionTable.TokToWord(PREEVALWORD);
            executionStack.push(preevalword);
            Evaluate();
            Evaluate();
        }
    }

    void Environment::AddPrimaryDefinition(Word word, 
        void (*handler)(Environment&)) {
        Word primarydefinitiontype 
            = definitionTable.TokToWord(PRIMARYDEFINITIONWORD);
        definitionTable[word].type = primarydefinitiontype;
        definitionTable[word].value.handler = handler;
    }

} // namespace huelang
