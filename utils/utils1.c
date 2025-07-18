#include "../minishell.h"

int count_tokens(token_t *tokens)
{
    int count = 0;
    while (tokens[count].type != NULL)
        count++;
    return count;
}
int is_redirection(const char *type)
{
    return (strcmp(type, "REDIR_IN") == 0 ||
            strcmp(type, "REDIR_OUT") == 0 ||
            strcmp(type, "REDIR_APPEND") == 0 ||
            strcmp(type, "REDIR_HEREDOC") == 0);
}

redir_type_t get_redir_type(const char *type)
{
    if (strcmp(type, "REDIR_IN") == 0)
        return REDIR_INPUT;
    if (strcmp(type, "REDIR_OUT") == 0)
        return REDIR_OUTPUT;
    if (strcmp(type, "REDIR_APPEND") == 0)
        return REDIR_APPEND;
    if (strcmp(type, "REDIR_HEREDOC") == 0)
        return REDIR_HEREDOC;
    return REDIR_INPUT;
}

command_t *create_command(void)
{
    command_t *cmd = malloc(sizeof(command_t));
    if (!cmd)
        return NULL;

    cmd->name = NULL;
    cmd->args = malloc(sizeof(char *) * MAX_ARGS);
    cmd->arg_count = 0;
    cmd->redirections = NULL;

    return cmd;
}

redirection_t *create_redirection(redir_type_t type, char *filename)
{
    redirection_t *redir = malloc(sizeof(redirection_t));
    if (!redir)
        return NULL;

    redir->type = type;
    redir->filename = ft_strdup(filename);
    redir->next = NULL;

    return redir;
}

int ft_strcmp(const char *s1, const char *s2)
{
    unsigned char c1, c2;
    while (*s1 || *s2)
    {
        c1 = (unsigned char)*s1;
        c2 = (unsigned char)*s2;
        if (c1 != c2)
            return c1 - c2;
        s1++;
        s2++;
    }
    return 0;
}

char *ft_strcpy(char *dest, const char *src)
{
    char *d = dest;
    while ((*d++ = *src++))
        ;
    return dest;
}