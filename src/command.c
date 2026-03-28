#include "command.h"
#include "external.h"
#include "platform.h"

#include "builtins.h"

void command_dispatch(char **argv, int argc) {
    if (argc == 0)
        return;
    const char *builtin_name = argv[0];
    const Command *builtin = find_builtin(builtin_name);
    if (builtin) {
        builtin->handler(argv, argc);
        return;
    }
    char buf[PATH_MAX];
    if (find_external_path(builtin_name, buf, sizeof(buf))) {
        exec_external(argv, argc);
        return;
    }
    fprintf(stderr, "%s: command not found\n", builtin_name);
}
