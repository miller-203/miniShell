#include "../minishell.h"

static void	print_export_env(t_env *env)
{
	t_env *tmp = env;
	while (tmp)
	{
		if (tmp->exported)
		{
			if (tmp->value)
				printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
			else
				printf("declare -x %s\n", tmp->key);
		}
		tmp = tmp->next;
	}
}

static void	export_set_var(char *arg, t_env **env)
{
	char *eq = ft_strchr(arg, '=');
	char *val;
	if (eq)
	{
		*eq = '\0';
		val = strip_quotes(eq + 1);
		*env = set_env_var(*env, arg, val, 1);
		free(val);
		*eq = '=';
	}
	else
	{
		t_env *cur = *env;
		int found = 0;
		while (cur)
		{
			if (ft_strcmp(cur->key, arg) == 0)
			{
				cur->exported = 1;
				found = 1;
				break;
			}
			cur = cur->next;
		}
		if (!found)
			*env = set_env_var(*env, arg, NULL, 1);
	}
}

int	builtin_export(char **args, t_env **env)
{
	int i = 1;
	if (!args[i])
	{
		print_export_env(*env);
		return (0);
	}
	while (args[i])
	{
		export_set_var(args[i], env);
		i++;
	}
	return (0);
}
