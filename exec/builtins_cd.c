#include "../minishell.h"

int builtin_cd(char **args, t_env *env) {
    const char *target = (args[1]) ? args[1] : NULL;
    if (!target) {
        t_env *tmp = env;
        while (tmp) {
            if (ft_strcmp(tmp->key, "HOME") == 0) {
                target = tmp->value;
                break;
            }
            tmp = tmp->next;
        }
        if (!target) {
            write(2, "cd: HOME not set\n", 17);
            return 1;
        }
    }
    if (chdir(target) != 0) {
        perror("cd");
        return 1;
    }
    return 0;
} 
