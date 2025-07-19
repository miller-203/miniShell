#include "../minishell.h"

static int	env_count(t_env *env)
{
	int		count;
	t_env	*tmp;

	count = 0;
	tmp = env;
	while (tmp)
	{
		if (tmp->key)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

static char	*env_to_str(const char *key, const char *value)
{
	size_t	len;
	char	*str;

	if (!key)
		return (NULL);
	if (!value)
		value = "";
	len = ft_strlen(key) + ft_strlen(value) + 2;
	str = malloc(len);
	if (!str)
		return (NULL);
	ft_strcpy(str, key);
	str[ft_strlen(key)] = '=';
	ft_strcpy(str + ft_strlen(key) + 1, value);
	return (str);
}

static void	free_environ_partial(char **environ, int i)
{
	int	j;

	j = 0;
	while (j < i)
		free(environ[j++]);
	free(environ);
}

char	**env_list_to_environ(t_env *env)
{
	int		count;
	char	**environ;
	t_env	*tmp;
	int		i;

	count = env_count(env);
	environ = malloc(sizeof(char *) * (count + 1));
	if (!environ)
		return (NULL);
	tmp = env;
	i = 0;
	while (tmp && i < count)
	{
		if (!tmp->key)
		{
			tmp = tmp->next;
			continue;
		}
		environ[i] = env_to_str(tmp->key, tmp->value);
		if (!environ[i])
			return (free_environ_partial(environ, i), NULL);
		i++;
		tmp = tmp->next;
	}
	return (environ[i] = NULL, environ);
}
