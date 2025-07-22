#include "../minishell.h"

int exec_pipeline(ast_node_t *node, t_env **env)
{
    int pipefd[2];
    int left_pid;
    int right_pid;

    if (pipe(pipefd) < 0)
        return (print_exec_error("pipe", NULL), 1);
    
    left_pid = fork_left_process(node, env, pipefd);
    if (left_pid < 0)
        return (1);
    right_pid = fork_right_process(node, env, pipefd, left_pid);
    if (right_pid < 0)
        return (1);
    close(pipefd[0]);
    close(pipefd[1]);
    return (wait_pipeline_children(left_pid, right_pid));
}

int	execute_ast(ast_node_t *node, t_env **env)
{
	if (!node)
		return (1);
	if (node->type == NODE_COMMAND)
		return (execute_command(node->data.command, env));
	else if (node->type == NODE_PIPELINE)
		return (exec_pipeline(node, env));
	return (1);
}
