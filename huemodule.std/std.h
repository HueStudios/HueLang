#ifndef STD_H
#define STD_H

#include <lhuelang/environment.h>
#include <lhuelang/core.h>
#include <iostream>

using namespace huelang;

extern "C" void ModuleEntry(Environment *env);

void __hello(Environment &env);

#endif