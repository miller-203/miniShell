#include "../minishell.h"
#include <stdlib.h>

void free_environ(char **env)
{
    int i = 0;
    if (!env)
        return;
    while (env[i])
        free(env[i++]);
    free(env);
}
