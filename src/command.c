#include "command.h"
#include "string_view.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
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

void cmd_cd(String_View args) {
    char *path = sv_to_cstr(args);
    if (chdir(sv_to_cstr(args)) != 0) {
        switch (errno) {
        case ENOENT:
            fprintf(stderr, "cd: no such file or directory: %s\n", path);
            break;
        case ENOTDIR:
            fprintf(stderr, "cd: not a directory: %s\n", path);
            break;
        case EACCES:
            fprintf(stderr, "cd: permission denied: %s\n", path);
            break;
        default:
            perror("chdir");
            break;
        }
    };
};

static const Command commands[] = {
    {"exit", cmd_exit}, {"echo", cmd_echo}, {"type", cmd_type}, {"pwd", cmd_pwd}, {"cd", cmd_cd},
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

void exec_external_path(String_View bin_name, String_View args) {
    pid_t pid = fork();

    if (pid < 0) {
        printf(SV_FMT ": error trying to execute bin\n", SV_ARG(args));
        return;
    }

    if (pid == 0) {
        char *bin_name_cstr = sv_to_cstr(bin_name);

        char *argv[64] = {0};
        size_t argc = 0;
        argv[argc++] = bin_name_cstr;
        while (args.len > 0) {
            String_View token = sv_chop_by_delim(&args, ' ');
            if (token.len == 0)
                continue;
            argv[argc++] = sv_to_cstr(token);
        }

        execvp(sv_to_cstr(bin_name), argv);
        perror(sv_to_cstr(bin_name));
        exit(127);
    }

    if (waitpid(pid, NULL, 0) < 0) {
        printf(SV_FMT ": error trying to execute bin\n", SV_ARG(args));
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

    char buf[PATH_MAX];
    if (find_external_path(cmd, buf, sizeof(buf))) {
        exec_external_path(cmd, args);
        return;
    }

    printf(SV_FMT ": command not found\n", SV_ARG(cmd));
}
