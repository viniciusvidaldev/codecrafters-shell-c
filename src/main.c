#include "parse.h"
#define SV_IMPLEMENTATION

#include "command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 256
#define MAX_ARGS 4096

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

        char token_storage[BUF_SIZE];
        char *cmd_argv[MAX_ARGS];

        size_t cmd_argc = parse_args(buf, token_storage, cmd_argv);
        command_dispatch(cmd_argv, cmd_argc);
    }

    return 0;
}
