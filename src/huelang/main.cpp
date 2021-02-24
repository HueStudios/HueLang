
#include <iostream>
#include <stdio.h>
#include "../libhuelang/value.h"
#include "../libhuelang/environment.h"
#include "../libhuelang/core.h"

using namespace std;
using namespace huelang;

int main(int argc, char const *argv[])
{
    Environment *env = new Environment();
    InitializeCore(*env);
    Word testword = env->definitionTable.TokToWord("test");
    env->executionStack.push(testword);
    env->Run();
    return 0;
}
