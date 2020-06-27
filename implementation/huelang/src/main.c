#if INTERFACE
#include "repl.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#endif

#include "main.h"

int main (int argc, const char *argv[]) {
  srand(time(0));
  Repl_Start();
}
