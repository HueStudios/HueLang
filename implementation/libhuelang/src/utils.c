#if INTERFACE

#include <string.h>
#include <stdlib.h>

#endif

#include "utils.h"

char *Utils_MallocConcat(char *a, char *b) {
  int len_a = strlen(a);
  int len_b = strlen(b);
  int len_final = len_a + len_b;
  char *result = malloc(sizeof(char) * len_final);
  result[0] = '\0';
  strcat(result, a);
  strcat(result, b);
  return result;
}
