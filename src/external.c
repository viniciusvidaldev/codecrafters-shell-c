#include "string_view.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define PATH_SEP ':'

bool find_external_path(const char *bin_name, char *out, size_t out_len) {
    const char *path_env = getenv("PATH");
    if (path_env == NULL)
        return false;
    String_View sv = sv_from(path_env);
    while (sv.len > 0) {
        String_View dir = sv_chop_by_delim(&sv, PATH_SEP);
        snprintf(out, out_len, SV_FMT "/%s", SV_ARG(dir), bin_name);
        if (access(out, X_OK) == 0)
            return true;
    }
    return false;
}

void exec_external(char **argv, int argc) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }
    if (pid == 0) {
        execvp(argv[0], argv);
        perror(argv[0]);
        exit(127);
    }
    if (waitpid(pid, NULL, 0) < 0) {
        perror("waitpid");
    }
}
