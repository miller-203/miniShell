#include "../minishell.h"

int builtin_env(t_env *env) {
    t_env *tmp = env;
    while (tmp) {
        printf("%s=%s\n", tmp->key, tmp->value);
        tmp = tmp->next;
    }
    return 0;
} 