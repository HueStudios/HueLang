#if INTERFACE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
typedef enum TokenizerModes {
    normalreadmode,
    stringreadmode,
    quotescapemode,
} TokenizerModes;
#endif

#include "tokenize.h"

void append_to_buffer (unsigned char **buffer, char to_add, int *len) {
    *len += 1;
    *buffer = realloc(*buffer, *len);
    (*buffer)[*len - 1] = to_add;
    //printf("%u", to_add);
}

char *read_token_from_filedesc(int fd, int *len) {
    unsigned char focus = 0;
    unsigned char prev_focus = 0;
    *len = 0;
    unsigned char *buffer = NULL;
    TokenizerModes mode = normalreadmode;
    while (1) {
        prev_focus = focus;
        unsigned int bytes_read = read(fd, &focus, 1);
        if (bytes_read == 0) {
            break;
        }
        if (focus == '"') {
            if ((*len == 0) | (mode == quotescapemode)) {
                append_to_buffer(&buffer, focus, len);
                mode = stringreadmode;
                continue;
            } else {
                mode = quotescapemode;
            }
        } else if (mode == quotescapemode) {
            break;
        }
        if (isspace(focus)) {
            if (*len == 0) return read_token_from_filedesc(fd, len);
            if (mode == normalreadmode) {
                break;
            }
        }
        append_to_buffer(&buffer, focus, len);
    }
    if (buffer == NULL) {
        return NULL;
    }
    append_to_buffer(&buffer, '\0', len);
    return buffer;
}
