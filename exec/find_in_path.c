#include "../minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static char	*get_path_from_env(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "PATH") == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return ("/bin:/usr/bin:/usr/local/bin");
}

static char	*check_directory_for_command(const char *dir, const char *cmd)
{
	char	full[1024];

	size_t dir_len = ft_strlen(dir);
	size_t cmd_len = ft_strlen(cmd);
	if (dir_len + 1 + cmd_len >= sizeof(full))
		return (NULL);
	ft_strcpy(full, dir);
	full[dir_len] = '/';
	ft_strcpy(full + dir_len + 1, cmd);
	full[dir_len + 1 + cmd_len] = '\0';
	if (access(full, X_OK) == 0)
		return (ft_strdup(full));
	return (NULL);
}

static char	*extract_next_dir(char *path, int *start)
{
	int		end;
	char	*dir;

	if (!path || !path[*start])
		return (NULL);
	end = *start;
	while (path[end] && path[end] != ':')
		end++;
	// implement substr
	int len = end - *start;
	dir = malloc(len + 1);
	if (!dir)
		return (NULL);
	memcpy(dir, path + *start, len);
	dir[len] = '\0';
	if (path[end])
		*start = end + 1;
	else
		*start = end;
	return (dir);
}

static char	*search_in_path_dirs(const char *cmd, const char *path)
{
	char	*paths;
	char	*dir;
	char	*result;
	int		start;

	paths = ft_strdup(path);
	if (!paths)
		return (NULL);
	start = 0;
	while ((dir = extract_next_dir(paths, &start)) != NULL)
	{
		result = check_directory_for_command(dir, cmd);
		free(dir);
		if (result)
		{
			free(paths);
			return (result);
		}
	}
	free(paths);
	return (NULL);
}

char	*find_in_path(const char *cmd, t_env *env)
{
	const char	*path;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path = get_path_from_env(env);
	return (search_in_path_dirs(cmd, path));
}