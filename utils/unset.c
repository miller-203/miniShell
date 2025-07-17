#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int unset_builtin(char **args, t_env **env) {
    int i = 1;
    while (args[i]) {
        t_env *cur = *env, *prev = NULL;
        while (cur) {
            if (strcmp(cur->key, args[i]) == 0) {
                if (prev) prev->next = cur->next;
                else *env = cur->next;
                free(cur->key); free(cur->value); free(cur);
                break;
            }
            prev = cur; cur = cur->next;
        }
        i++;
    }
    return 0;
}