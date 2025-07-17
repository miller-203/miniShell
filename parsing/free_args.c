#include "../minishell.h"

void ft_free_args(char **args)
{
    int i = 0;
    while (args[i])
    {
        free(args[i]);
        i++;
    }
    free(args);
    args = NULL;
}

void free_tokens(token_t *tokens)
{
    if (!tokens)
        return;

    int i = 0;
    while (tokens[i].type != NULL)
    {
        free(tokens[i].value);
        i++;
    }
    free(tokens);
}

void free_ast(ast_node_t *node)
{
    if (!node)
        return;

    if (node->type == NODE_COMMAND)
    {
        if (node->data.command)
        {
            free(node->data.command->name);
            int i = 0;
            while (i < node->data.command->arg_count)
            {
                free(node->data.command->args[i]);
                i++;
            }
            free(node->data.command->args);
            redirection_t *redir = node->data.command->redirections;
            while (redir)
            {
                redirection_t *next = redir->next;
                free(redir->filename);
                free(redir);
                redir = next;
            }
            free(node->data.command);
        }
    }
    else if (node->type == NODE_PIPELINE)
    {
        free_ast(node->data.pipeline.left);
        free_ast(node->data.pipeline.right);
    }

    free(node);
}