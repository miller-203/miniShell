#include "minishell.h"

void print_ast(ast_node_t *node, int depth)
{
    if (!node)
        return;

    int i = 0;
    while (i < depth)
    {
        printf("  ");
        i++;
    }

    if (node->type == NODE_COMMAND)
    {
        printf("COMMAND: %s\n", node->data.command->name ? node->data.command->name : "NULL");
        for (int i = 0; i < node->data.command->arg_count; i++)
        {
            for (int j = 0; j < depth + 1; j++)
                printf("  ");
            printf("ARG[%d]: %s\n", i, node->data.command->args[i]);
        }

        redirection_t *redir = node->data.command->redirections;
        while (redir)
        {
            int j = 0;
            while (j < depth + 1)
            {
                printf("  ");
                j++;
            }
            printf("REDIR: %d -> %s\n", redir->type, redir->filename);
            redir = redir->next;
        }
    }
    else if (node->type == NODE_PIPELINE)
    {
        printf("PIPELINE:\n");
        print_ast(node->data.pipeline.left, depth + 1);
        print_ast(node->data.pipeline.right, depth + 1);
    }
}

int ft_work_parse_execute(char *line, t_env **env)
{
    (void)env;
    token_t *tokens = tokenize(line);
    if (!tokens)
        return 1;
    ast_node_t *ast = parse(tokens);
    if (!ast)
    {
        free_tokens(tokens);
        return 1;
    }
    // printf("Parsed tokens:\n");
    // int i = 0;
    // while (tokens[i].type)
    // {
    //     printf("Type: %s, Value: %s\n", tokens[i].type, tokens[i].value);
    //     i++;
    // }
    printf("AST Structure:\n");
    print_ast(ast, 0);
    free(tokens);
    return 0;
}

int main(int argc, char **argv, char **env)
{
    char *line;

    (void)argc;
    (void)argv;

    t_env *copy_env;
    ft_copy_env(env, &copy_env);
    while (1)
    {
        line = readline("minishell-1.0$ ");

        if (line == NULL)
        {
            write(1, "exit\n", 5);
            break;
        }
        if (line[0] != '\0')
        {
            add_history(line);

            if (strcmp(line, "history -c") == 0)
                rl_clear_history();
            else if (strcmp(line, "history") == 0)
            {
                HIST_ENTRY **hist_list = history_list();
                if (hist_list)
                {
                    int i = 0;
                    while (hist_list[i])
                    {
                        printf("\t%d  %s\n", i + history_base, hist_list[i]->line);
                        i++;
                    }
                }
            }
            else
                ft_work_parse_execute(line, &copy_env);
        }
        free(line);
    }
    ft_free_env(&copy_env);

    return 0;
}
