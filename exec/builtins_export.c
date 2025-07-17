#include "../minishell.h"

int builtin_export(char **args, t_env **env) {
    // Call your export logic here (previously export_builtin)
    return export_builtin(args, env);
} 