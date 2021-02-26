#include "interpreter.h"

void Interpreter::InterpretLine (string line) {
    vector<string> tokens;
    split(tokens, line, is_any_of(" "));
    
    for (string& tok: tokens) {
        Word tokword = env.definitionTable.TokToWord(tok);
        env.executionStack.push(tokword);
        env.Run();
    }
}