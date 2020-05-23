#if INTERFACE

#include <stdlib.h>
#include "type.h"

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

void *ValueStack_GetAbsolutePointer(ValueStack *self) {
  void *new = self->data + self->pointer;
  return new;
}

void *ValueStack_PushAlloc(ValueStack *self, unsigned int size) {
  void *result = ValueStack_GetAbsolutePointer(self);
  self->pointer += size;
  return result;
}

// TODO this
Word *ValueStack_Pop(ValueStack *self) {
  self->pointer -= sizeof(Word);
  return self->pointer;
}
