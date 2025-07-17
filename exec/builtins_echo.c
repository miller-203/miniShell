#include "../minishell.h"

int builtin_echo(char **args) {
    int nflag = 0, i = 1;
    if (args[1] && ft_strcmp(args[1], "-n") == 0) {
        nflag = 1;
        i = 2;
    }
    for (; args[i]; i++) {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
    }
    if (!nflag)
        printf("\n");
    return 0;
} 