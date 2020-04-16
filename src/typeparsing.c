#if INTERFACE
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#endif 

#include "typeparsing.h"

long *token_to_long(char *token) {
  char *end;
  if (token[0] == '\0' || isspace(token[0])) {
    return NULL;
  }
  long *result = malloc(sizeof(long));
  *result = strtol(token, &end, 10);
  if (*end != '\0') {
    free(result);
    return NULL;
  }
  return result;
}

char *token_to_string (char *token, unsigned char remove_quotes) {
  unsigned int length = strlen(token);
  if ((token[0] == '"') & (token[length - 1] == '"')) {
    unsigned int new_length = length;
    unsigned int start = 0;
    if (remove_quotes) {
      new_length -= 2;
      start = 1;
    }
    char *result = malloc(sizeof(char) * new_length);
    for (unsigned int i = 0; i < new_length; i++) {
      unsigned int actual_index = i + start;
      result[i] = token[actual_index];
    }
    return result;
  }
  return NULL;
}

char *token_to_quote (char *token, unsigned char remove_quotes) {
  unsigned int length = strlen(token);
  if ((token[0] == ':')) {
    unsigned int new_length = length;
    unsigned int start = 0;
    if (remove_quotes) {
      new_length -= 1;
      start = 1;
    }
    char *result = malloc(sizeof(char) * new_length);
    for (unsigned int i = 0; i < new_length; i++) {
      unsigned int actual_index = i + start;
      result[i] = token[actual_index];
    }
    return result;
  }
  return NULL;  
}

// HACK
void *token_as_whatever (char *token, unsigned char remove_quotes) {
  void *result = NULL;
  result = token_to_long(token);
  if (result != NULL) return result;
  result = token_to_string(token, remove_quotes);
  if (result != NULL) return result;
  result = token_to_quote(token, remove_quotes);
  return result; 
}