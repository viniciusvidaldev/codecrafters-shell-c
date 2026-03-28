#include "command.h"
#include "external.h"
#include "platform.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void builtin_type(char **argv, int argc);

static void builtin_exit(char **argv, int argc) { exit(0); }

static void builtin_echo(char **argv, int argc) {
    for (int i = 1; i < argc; i++) {
        if (i > 1)
            printf(" ");
        printf("%s", argv[i]);
    }
    printf("\n");
}

static void builtin_pwd(char **argv, int argc) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return;
    }
    printf("%s\n", cwd);
}

static void builtin_cd(char **argv, int argc) {
    const char *path;
    if (argc < 2 || strcmp(argv[1], "~") == 0) {
        path = getenv("HOME");
        if (path == NULL) {
            fprintf(stderr, "cd: HOME not set\n");
            return;
        }
    } else {
        path = argv[1];
    }
    if (chdir(path) != 0) {
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
    }
}

static const Command commands[] = {
    {"exit", builtin_exit}, {"echo", builtin_echo}, {"type", builtin_type},
    {"pwd", builtin_pwd},   {"cd", builtin_cd},
};

static const size_t commands_len = sizeof(commands) / sizeof(commands[0]);

const Command *find_builtin(const char *name) {
    for (size_t i = 0; i < commands_len; i++) {
        if (strcmp(name, commands[i].name) == 0) {
            return &commands[i];
        }
    }
    return NULL;
}

static void builtin_type(char **argv, int argc) {
    if (argc < 2) {
        fprintf(stderr, "type: missing argument\n");
        return;
    }
    const char *name = argv[1];
    if (find_builtin(name)) {
        printf("%s is a shell builtin\n", name);
        return;
    }
    char buf[PATH_MAX];
    if (find_external_path(name, buf, sizeof(buf))) {
        printf("%s is %s\n", name, buf);
        return;
    }
    printf("%s: not found\n", name);
}
