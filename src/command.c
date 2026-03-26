#include "command.h"
#include "string_view.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef unix
#include <linux/limits.h>
#else
#include <sys/syslimits.h>
#endif /* ifdef unix */

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
    {"pwd", cmd_pwd},
};

static const size_t commands_len = sizeof(commands) / sizeof(commands[0]);

void command_dispatch(String_View cmd, String_View args) {
    for (size_t i = 0; i < commands_len; i++) {
        if (sv_eq(cmd, sv_from(commands[i].name))) {
            commands[i].handler(args);
            return;
        }
    }

    printf(SV_Fmt ": command not found\n", SV_Arg(cmd));
}
