#define SV_IMPLEMENTATION
#include "string_view.h"

#include "command.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 256

int main(int argc, char *argv[]) {
    // Flush after every printf
    setbuf(stdout, NULL);

    while (1) {
        printf("$ ");
        char buf[BUF_SIZE];
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            if (feof(stdin)) {
                printf("\n");
                return EXIT_SUCCESS;
            }

            perror("fgets");
            return EXIT_FAILURE;
        }
        buf[strcspn(buf, "\n")] = '\0';

        String_View input = sv_from(buf);

        char *argv[ARG_MAX];
        size_t argc = 0;
        while (input.len > 0) {
            sv_trim(&input);
            String_View token = sv_chop_by_delim(&input, ' ');
            argv[argc++] = sv_to_cstr(token);
        }

        command_dispatch(argv, argc);
    }

    return 0;
}
