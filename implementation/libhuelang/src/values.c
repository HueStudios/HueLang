#if INTERFACE

typedef struct Environment Environment;

#include <stdlib.h>
#include "type.h"
#include "environment.h"

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

void ValueStack_ResolveStackGrowth(ValueStack *self) {
  if (self->pointer <= self->size) {
    self->size *= 2;
    ValueStack_ResolveStackGrowth(self);
  }
}

void *ValueStack_PushAlloc(ValueStack *self, unsigned int size) {
  void *result = ValueStack_GetAbsolutePointer(self);
  self->pointer += size;
  ValueStack_ResolveStackGrowth(self);
  self->data = realloc(self->data, self->size);
  return result;
}

Word *ValueStack_Pop(ValueStack *self, Environment *env) {
  self->pointer -= sizeof(Word);
  Word *result = ValueStack_GetAbsolutePointer(self);
  int size = Types_ResolveTypeSize(env, result);
  self->pointer -= size;
  return result;
}
