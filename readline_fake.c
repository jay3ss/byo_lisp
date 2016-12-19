#include "readline_fake.h"

static char buffer[2048];

char* readline(char* prompt) {
    fputs(prompt, stdout);
    fgets(buffer, 2048, stdin);
    char* cpy = malloc(strlen(buffer)+1);
    cpy[strlen(cpy)-1] = '\0';
    return cpy;
}
