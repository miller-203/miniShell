#include "../minishell.h"

int	builtin_export(char **args, t_env **env)
{
	return (export_builtin(args, env));
}
