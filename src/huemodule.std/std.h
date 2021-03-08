#ifndef STD_H
#define STD_H

#include "../libhuelang/environment.h"
#include "../libhuelang/core.h"
#include <iostream>

using namespace huelang;

extern "C" void ModuleEntry(Environment *env);

void __hello(Environment &env);

#endif