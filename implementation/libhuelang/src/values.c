#if INTERFACE

#include <stdlib.h>

typedef struct ValueStack {
  unsigned long pointer;
  unsigned long size;
  unsigned char *data;
} ValueStack;

#define INITIALVALUESTACKSIZE 0xFF

#endif

#include "values.h"

ValueStack *ValueStack_Create() {
  ValueStack *self = malloc(sizeof(ValueStack));
  self->pointer = 0;
  self->size = INITIALVALUESTACKSIZE;
  self->data = malloc(sizeof(char) * self->size);
  return self;
}
