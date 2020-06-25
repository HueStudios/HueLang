#if INTERFACE

#include <string.h>
#include <stdlib.h>
typedef struct CrudeStream {
    char *data;
    unsigned int length;
    unsigned int index;
} CrudeStream;

#endif

#include "crudestream.h"

unsigned int CrudeStream_Read(CrudeStream *stream, char *buffer, 
  unsigned int len) {
  unsigned int available = stream->length - stream->index;
  unsigned int to_read = len;
  if (len > available) {
    if (available == 0) {
      return 0;
    } 
    to_read = available;
  }
  memcpy(buffer, stream->data + stream->index, to_read);
  stream->index += to_read;
  return to_read;
}

