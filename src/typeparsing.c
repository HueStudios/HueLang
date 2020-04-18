#if INTERFACE
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
typedef enum ValueType {
  stringtype,
  longtype,
  quotetype,
  symboltype,
  nonetype
} ValueType;
typedef struct ArbitraryValue {
  ValueType type;
  void *value;
  unsigned char dynamic;
} ArbitraryValue;
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
      new_length -= 1;
      start = 1;
    }
    char *result = malloc(sizeof(char) * new_length);
    for (unsigned int i = 0; i < new_length - 1; i++) {
      unsigned int actual_index = i + start;
      result[i] = token[actual_index];
    }
    result[new_length - 1] = '\0';
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
      start = 1;
    }
    char *result = malloc(sizeof(char) * new_length);
    for (unsigned int i = 0; i < new_length - 1; i++) {
      unsigned int actual_index = i + start;
      result[i] = token[actual_index];
    }
    result[new_length - 1] = '\0';
    return result;
  }
  return NULL;  
}

char *token_to_symbol (char *token) {
  char *result;
  unsigned int length = strlen(token) + 1;
  result = malloc(sizeof(char) * length);
  strcpy(result, token);
  return result;
}

// HACK
ArbitraryValue *token_to_whatever (char *token, unsigned char remove_quotes) {
  ArbitraryValue *result = malloc(sizeof(ArbitraryValue));
  result->dynamic = 1;
  result->value = token_to_long(token);
  result->type = longtype;
  if (result->value != NULL) return result;
  result->value = token_to_string(token, remove_quotes);
  result->type = stringtype;
  if (result->value != NULL) return result;
  result->value = token_to_quote(token, remove_quotes);
  result->type = quotetype;
  if (result->value != NULL) return result;
  // The token must be a symbol
  result->value = token_to_symbol(token);
  result->type = symboltype;
  return result; 
}

void cleanup_value(ArbitraryValue *self) {
  if (self->dynamic) {
      free(self->value);
      free(self);
    }
}