#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>

size_t parse_args(const char *input, char *storage, char **argv) {
    size_t argc = 0;
    char *out = storage;
    char *token_start = NULL;
    bool in_single = false, in_double = false;

    for (const char *p = input; *p; p++) {
        char c = *p;

        if (c == '\'' && !in_double) {
            in_single = !in_single;
            continue;
        }
        if (c == '"' && !in_single) {
            in_double = !in_double;
            continue;
        }

        if (in_single || in_double || !isspace(c)) {
            if (!token_start) {
                token_start = out;
            }

            *out++ = c;
        } else if (token_start) {
            *out++ = '\0';
            argv[argc++] = token_start;
            token_start = NULL;
        }
    }
    if (token_start) {
        *out = '\0';
        argv[argc++] = token_start;
    }
    argv[argc] = NULL;
    return argc;
}
