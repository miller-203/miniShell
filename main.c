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
        int i = 0;
        while (i < node->data.command->arg_count)
        {
            int j = 0;
            while (j < depth + 1)
            {
                printf("  ");
                j++;
            }
            printf("ARG[%d]: %s\n", i, node->data.command->args[i]);
            i++;
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

int	ft_work_parse_execute(char *line, t_env **env)
{
    ast_node_t *ast = parse_input_line(line);
    if (!ast)
        return 1;
    print_ast(ast, 0);
    int status = execute_ast(ast, env);
    free_ast(ast);
    return status;
}

void sigint_handler(int signo)
{
    (void)signo;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

int is_builtin(const char *name) {
    return (strcmp(name, "cd") == 0 ||
            strcmp(name, "pwd") == 0 ||
            strcmp(name, "echo") == 0 ||
            strcmp(name, "env") == 0 ||
            strcmp(name, "export") == 0 ||
            strcmp(name, "unset") == 0 ||
            strcmp(name, "exit") == 0);
}

int main(int argc, char **argv, char **env)
{
    char *line;

    (void)argc;
    (void)argv;

    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);

    t_env *copy_env;
    ft_copy_env(env, &copy_env);
    
    while (1)
    {
        line = readline("minishell$ ");

        if (line == NULL)
        {
            write(1, "exit\n", 5);
            break;
        }
        
        if (line[0] != '\0')
        {
            add_history(line);

            if (ft_strcmp(line, "history -c") == 0)
                rl_clear_history();
            else if (ft_strcmp(line, "history") == 0)
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