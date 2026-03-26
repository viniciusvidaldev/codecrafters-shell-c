#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syslimits.h>
#include <unistd.h>

#define BUF_SIZE 256

int main(int argc, char *argv[]) {
    // Flush after every printf
    setbuf(stdout, NULL);

    while (1) {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd");
        };

        printf("%s\n", cwd);
        printf("λ ");
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

        printf("%s: command not found\n\n", buf);
    }

    return 0;
}
