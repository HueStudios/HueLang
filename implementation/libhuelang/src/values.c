#if INTERFACE

typedef struct ValueStack {
  unsigned long pointer;
  unsigned long size;
  unsigned char *data;
}

#define INITIALVALUESTACKSIZE 0xFF

#endif

