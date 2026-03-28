#include <stdbool.h>
#include <stddef.h>

size_t parse_args(const char *input, char *storage, char **argv) {
    size_t argc = 0;
    size_t storage_offset = 0;
    size_t token_len = 0;
    bool in_quotes = false;

    for (const char *p = input; *p != '\0'; p++) {
        char c = *p;

        if (c == '\'') {
            in_quotes = !in_quotes;
            continue;
        }

        if (!in_quotes && c == ' ') {
            if (token_len > 0) {
                storage[storage_offset + token_len] = '\0';
                argv[argc++] = &storage[storage_offset];
                storage_offset += token_len + 1;
                token_len = 0;
            }
            continue;
        }

        storage[storage_offset + token_len] = c;
        token_len++;
    }

    if (token_len > 0) {
        storage[storage_offset + token_len] = '\0';
        argv[argc++] = &storage[storage_offset];
    }

    argv[argc] = NULL;
    return argc;
}
