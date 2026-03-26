#define SV_IMPLEMENTATION
#include "string_view.h"

#include "command.h"

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

        String_View args = sv_from(buf);
        String_View cmd = sv_chop_by_delim(&args, ' ');

        command_dispatch(cmd, args);
    }

    return 0;
}
