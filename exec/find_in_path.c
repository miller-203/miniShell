#include "../minishell.h"

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
	return (NULL);
}

/*
** check_directory_for_command
**
** This function constructs a full path by joining the given directory (dir)
** and the command name (cmd) with a '/' separator. It then checks if the
** resulting path exists and is executable using the access() system call.
** If the file is found and is executable, it returns a newly allocated
** string containing the full path. Otherwise, it returns NULL.
**
** Example:
**   check_directory_for_command("/bin", "ls")
**   -> returns "/bin/ls" if it exists and is executable, else NULL.
*/
static char	*check_directory_for_command(const char *dir, const char *cmd)
{
	char	full[1024];
	size_t	dir_len;
	size_t	cmd_len;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
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
	int		len;
	char	*dir;

	if (!path || !path[*start])
		return (NULL);
	end = *start;
	while (path[end] && path[end] != ':')
		end++;
	len = end - *start;
	dir = malloc(len + 1);
	if (!dir)
		return (NULL);
	ft_strncpy_safe(dir, path + *start, len);
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
		// printf("dir: %s\n", dir);
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
	// printf("path: %s\n", path);
	return (search_in_path_dirs(cmd, path));
}