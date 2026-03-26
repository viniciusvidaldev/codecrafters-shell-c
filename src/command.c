#include "command.h"
#include "string_view.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef unix
#include <linux/limits.h>
#else
#include <sys/syslimits.h>
#endif /* ifdef unix */

#define PATH_SEP ':'

void cmd_type(String_View args);
void cmd_exit(String_View args) { exit(0); };
void cmd_echo(String_View args) { sv_println(args); };
void cmd_pwd(String_View args) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
    };

    printf("%s\n", cwd);
};

static const Command commands[] = {
    {"exit", cmd_exit},
    {"echo", cmd_echo},
    {"type", cmd_type},
    {"pwd", cmd_pwd},
};

static const size_t commands_len = sizeof(commands) / sizeof(commands[0]);

static const Command *find_cmd(String_View name) {
    for (size_t i = 0; i < commands_len; i++) {
        if (sv_eq(name, sv_from(commands[i].name))) {
            return &commands[i];
        }
    }

    return NULL;
}

bool find_external_path(String_View bin_name, char *out, size_t out_len) {
    const char *path_env = getenv("PATH");
    if (path_env == NULL) {
        return false;
    }

    String_View sv = sv_from(path_env);
    while (sv.len > 0) {
        String_View dir = sv_chop_by_delim(&sv, PATH_SEP);
        snprintf(out, out_len, SV_FMT "/" SV_FMT, SV_ARG(dir), SV_ARG(bin_name));

        if (access(out, X_OK) == 0) {
            return true;
        }
    }

    return false;
}

void exec_external_path(String_View args) {
    char buf[PATH_MAX];
    if (find_external_path(args, buf, sizeof(buf))) {
        printf(SV_FMT " is %s\n", SV_ARG(args), buf);
        return;
    }
}

void cmd_type(String_View args) {
    const Command *builtin_cmd = find_cmd(args);
    if (builtin_cmd) {
        printf(SV_FMT " is a shell builtin\n", SV_ARG(args));
        return;
    }

    char buf[PATH_MAX];
    if (find_external_path(args, buf, sizeof(buf))) {
        printf(SV_FMT " is %s\n", SV_ARG(args), buf);
        return;
    }

    printf(SV_FMT ": not found\n", SV_ARG(args));
}

void command_dispatch(String_View cmd, String_View args) {
    const Command *builtin_cmd = find_cmd(cmd);

    if (builtin_cmd) {
        builtin_cmd->handler(args);
        return;
    }

    printf(SV_FMT ": command not found\n", SV_ARG(cmd));
}
