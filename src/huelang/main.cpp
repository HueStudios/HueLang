
#include <iostream>
#include <stdio.h>
#include "../libhuelang/environment.h"

using namespace std;

int main(int argc, char const *argv[])
{
    Environment *env = new Environment();
    env->hue();
    return 0;
}
