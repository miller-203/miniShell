#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

t_env	*set_env_var(t_env *env, const char *key, const char *value)
{
	t_env	*cur;
	t_env	*new;

	cur = env;
	while (cur)
	{
		if (strcmp(cur->key, key) == 0)
		{
			free(cur->value);
			cur->value = ft_strdup(value);
			return (env);
		}
		if (!cur->next)
			break ;
		cur = cur->next;
	}
	new = malloc(sizeof(t_env));
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = NULL;
	if (cur)
		cur->next = new;
	else
		env = new;
	return (env);
}

static char	*strip_quotes(const char *val)
{
	size_t	len;
	char	*res;

	len = ft_strlen(val);
	printf("val-before: %s\n", val);
	printf("len: %zu\n", len);
	if (len >= 2 && ((val[0] == '"' && val[len - 1] == '"')
			|| (val[0] == '\'' && val[len - 1] == '\'')))
	{
		res = malloc(len - 1);
		if (!res)
			return (NULL);
		ft_strncpy_safe(res, val + 1, len - 2);
		res[len - 2] = '\0';
		printf("res: %s\n", res);
		return (res);
	}
	return (ft_strdup(val));
}

static int	export_print_env(t_env *env)
{
	t_env *tmp = env;
	while (tmp)
	{
		printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}

static int	export_set_vars(char **args, t_env **env)
{
	int		i;
	char	*eq;
	char	*val;

    i = 1;
	while (args[i])
	{
		printf("args[%d]:: %s\n", i, args[i]);
		eq = ft_strchr(args[i], '=');
		if (!eq)
		{
			i++;
			continue ;
		}
		if (!*(eq + 1))
			return (print_exec_error("export", "not a valid identifier"), 1);	
		*eq = 0;
		val = strip_quotes(eq + 1);
		*env = set_env_var(*env, args[i], val);
		free(val);
		*eq = '=';
		i++;
	}
	return (0);
}

int	export_builtin(char **args, t_env **env)
{
	if (!args[1])
		return export_print_env(*env);
	return export_set_vars(args, env);
}
