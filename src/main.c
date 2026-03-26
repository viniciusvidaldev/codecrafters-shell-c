#define SV_IMPLEMENTATION
#include "string_view.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef unix
#include <linux/limits.h>
#else
#include <sys/syslimits.h>
#endif /* ifdef unix */

#define BUF_SIZE 256

int main(int argc, char *argv[]) {
    // Flush after every printf
    setbuf(stdout, NULL);

    while (1) {
        // char cwd[PATH_MAX];
        // if (getcwd(cwd, sizeof(cwd)) == NULL) {
        //     perror("getcwd");
        // };

        // printf("%s\n", cwd);
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

        String_View cmd = sv_from(buf);
        if (sv_eq(cmd, sv_from("exit"))) {
            return 0;
        }

        printf("%s: command not found\n", buf);
    }

    return 0;
}
