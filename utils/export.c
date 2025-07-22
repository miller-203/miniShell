#include "../minishell.h"

t_env	*set_env_var(t_env *env, const char *key, const char *value, int exported)
{
	t_env	*cur;
	t_env	*new;

	cur = env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (cur->value)
			{
				free(cur->value);
				cur->value = NULL;
			}
			if (value)
				cur->value = ft_strdup(value);
			if (exported)
				cur->exported = 1;
			return (env);
		}
		if (!cur->next)
			break ;
		cur = cur->next;
	}
	new = malloc(sizeof(t_env));
	if (!new)
		return (env);
	new->key = ft_strdup(key);
	new->value = (value) ? ft_strdup(value) : NULL;
	new->exported = exported;
	new->next = NULL;
	if (cur)
		cur->next = new;
	else
		env = new;
	return (env);
}

char	*strip_quotes(const char *val)
{
	size_t	len;
	char	*res;

	len = ft_strlen(val);
	if (len >= 2 && ((val[0] == '"' && val[len - 1] == '"')
			|| (val[0] == '\'' && val[len - 1] == '\'')))
	{
		res = malloc(len - 1);
		if (!res)
			return (NULL);
		ft_strncpy_safe(res, val + 1, len - 2);
		res[len - 2] = '\0';
		return (res);
	}
	return (ft_strdup(val));
}

int	export_builtin(char **args, t_env **env)
{
	int		i;
	t_env	*tmp;
	char	*eq;
	char	*val;
	t_env	*cur;
	int		found;

	i = 1;
	if (!args[i])
	{
		tmp = *env;
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
		return (0);
	}
	while (args[i])
	{
		eq = ft_strchr(args[i], '=');
		if (eq)
		{
			*eq = '\0';
			val = strip_quotes(eq + 1);
			*env = set_env_var(*env, args[i], val, 1);
			free(val);
			*eq = '=';
		}
		else
		{
			cur = *env;
			found = 0;
			while (cur)
			{
				if (ft_strcmp(cur->key, args[i]) == 0)
				{
					cur->exported = 1;
					found = 1;
					break ;
				}
				cur = cur->next;
			}
			if (!found)
				*env = set_env_var(*env, args[i], NULL, 1);
		}
		i++;
	}
	return (0);
}