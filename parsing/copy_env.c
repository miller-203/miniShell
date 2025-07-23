#include "../minishell.h"

void ft_free_env(t_env **env)
{
    t_env *current;
    t_env *next;

    if (!env || !*env)
        return;

    current = *env;
    while (current)
    {
        next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
    *env = NULL;
}


t_env	*find_last_node(t_env *head)
{
	if (head == NULL)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

static void free_split(char **split)
{
    int i = 0;
    if (!split) return;
    while (split[i])
        free(split[i++]);
    free(split);
}

static void	append_node(t_env **env, char *key, char *value)
{
    t_env *node = NULL;
    t_env *last_node = NULL;
	if (!env)
		return ;
	node = malloc(sizeof(t_env));
	if (!node)
		return ;
	node->next = NULL;
    node->key = key;
	node->value = value;
    node->exported = 1;
	if (*env == NULL)
		*env = node;
	else
	{
		last_node = find_last_node(*env);
		if (last_node)
			last_node->next = node;
	}
}

void ft_copy_env(char **env, t_env **copy_env)
{
    int i = 0;
    char **split = NULL;
    while (env[i])
    {
        split = ft_split(env[i], '=');
        if (!split)
        {
            ft_free_env(copy_env);
            return;
        }
        append_node(copy_env, ft_strdup(split[0]), ft_strdup(split[1] ? split[1] : ""));
        free_split(split);
        i++;
    }
}