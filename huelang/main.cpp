
#include <iostream>
#include <stdio.h>
#include <lhuelang/value.h>
#include <lhuelang/environment.h>
#include <lhuelang/core.h>
#include "interpreter.h"
#include <readline/readline.h>
#include <readline/history.h>
#include "logo.h"

using namespace std;
using namespace huelang;

int main(int argc, char const *argv[])
{

    printf("%s  HueLang version pre-alpha\n\n",huelang_txt);
    Interpreter interpreter;
    InitializeCore(interpreter.env);

    rl_bind_key('\t', rl_insert);
    char* buf;

    while ((buf = readline(">> ")) != NULL) {
        int len = strlen(buf);
        if (len > 0) {
            add_history(buf);
        }

        interpreter.InterpretLine(buf);

        // readline malloc's a new buffer every time.
        free(buf);
    }

    return 0;
}
