#ifndef EXTERNAL_H
#define EXTERNAL_H

#include <stdbool.h>
#include <stddef.h>

bool find_external_path(const char *bin_name, char *out, size_t out_len);
void exec_external(char **argv, int argc);

#endif
