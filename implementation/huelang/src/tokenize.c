#if INTERFACE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#endif

#include "tokenize.h"

// Append a character to a character buffer
void Tokens_AppendToBuffer (char **buffer, char to_add, unsigned int *len) {
    // This is not ideal and should be optimized.
    // It will be horribly slow with big inputs. 
    // In fact, perhaps strtok should be used instead of this whole thing...
    *len += 1;
    *buffer = realloc(*buffer, *len);
    (*buffer)[*len - 1] = to_add;
}

// Read a single token from a file descriptor
char *Tokens_ReadTokFromFDesc(int fd, unsigned int *len) {
    char focus = 0;
    *len = 0;
    char *buffer = NULL;
    while (1) {
        unsigned int bytes_read = read(fd, &focus, 1);
        if (bytes_read == 0) {
            break;
        }
        if (isspace(focus)) {
            if (*len == 0) return Tokens_ReadTokFromFDesc(fd, len);
            break;
        }
        Tokens_AppendToBuffer(&buffer, focus, len);
    }
    if (buffer == NULL) {
        return NULL;
    }
    Tokens_AppendToBuffer(&buffer, '\0', len);
    return buffer;
}