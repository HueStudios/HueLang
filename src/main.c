#if INTERFACE
#include "tokenize.h"
#include<stdio.h>
#endif

#include "main.h"

void main(char **args) {
    while (1)
    {
        unsigned int len = 0;
        char *tok = read_token_from_filedesc(0,&len);
        printf("Token: %u %s\n", len, tok);
    }
}