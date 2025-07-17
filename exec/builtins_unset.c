#include "../minishell.h"

int builtin_unset(char **args, t_env **env) {
    // Call your unset logic here (previously unset_builtin)
    return unset_builtin(args, env);
} 