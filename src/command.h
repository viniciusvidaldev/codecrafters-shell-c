#ifndef COMMAND_H
#define COMMAND_H
#include "string_view.h"

typedef struct {
    const char *name;
    void (*handler)(String_View args);
} Command;

void command_dispatch(String_View cmd, String_View args);

#endif // COMMAND_H
