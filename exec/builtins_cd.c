#include "../minishell.h"

static const char *get_cd_target(char **args, t_env *env)
{
	t_env		*tmp;

	if (args[1])
		return args[1];
	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "HOME") == 0)
			return tmp->value;
		tmp = tmp->next;
	}
	return NULL;
}

int	builtin_cd(char **args, t_env *env)
{
	const char *target = get_cd_target(args, env);

	if (!target)
	{
		write(2, "cd: HOME not set\n", 17);
		return (1);
	}
	if (chdir(target) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}
