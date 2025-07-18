#include "../minishell.h"

int	builtin_unset(char **args, t_env **env)
{
	return (unset_builtin(args, env));
}
