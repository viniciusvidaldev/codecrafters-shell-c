#ifndef BUILTINS_H
#define BUILTINS_H

#include "command.h"

const Command *find_builtin(const char *name);

void builtin_exit(char **argv, int argc);
void builtin_echo(char **argv, int argc);
void builtin_pwd(char **argv, int argc);
void builtin_cd(char **argv, int argc);
void builtin_type(char **argv, int argc);

#endif
