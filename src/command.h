#ifndef COMMAND_H
#define COMMAND_H
#include "string_view.h"

typedef struct {
    const char *name;
    void (*handler)(char **argv, int argc);
} Command;

void command_dispatch(char **argv, int argc);

#endif // COMMAND_H
