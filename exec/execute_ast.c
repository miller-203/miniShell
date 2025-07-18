#include "../minishell.h"
#include <unistd.h>
#include <signal.h>

static int	exec_pipeline_left(ast_node_t *node, t_env **env, int *pipefd)
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	exit(execute_ast(node->data.pipeline.left, env));
}

static int	exec_pipeline_right(ast_node_t *node, t_env **env, int *pipefd)
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	exit(execute_ast(node->data.pipeline.right, env));
}

static int	wait_pipeline_children(int left_pid, int right_pid)
{
	int	status1;
	int	status2;

	waitpid(left_pid, &status1, 0);
	waitpid(right_pid, &status2, 0);
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	return (1);
}

static int fork_left_process(ast_node_t *node, t_env **env, int *pipefd)
{
    int left_pid;
    
    left_pid = fork();
    if (left_pid == 0)
        exec_pipeline_left(node, env, pipefd);
    else if (left_pid < 0)
    {
        print_exec_error("fork left", NULL);
        close(pipefd[0]);
        close(pipefd[1]);
        return (-1);
    }
    return (left_pid);
}

static int fork_right_process(ast_node_t *node, t_env **env, int *pipefd, int left_pid)
{
    int right_pid;
    
    right_pid = fork();
    if (right_pid == 0)
        exec_pipeline_right(node, env, pipefd);
    else if (right_pid < 0)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(left_pid, NULL, 0);
        print_exec_error("fork right", NULL);
        return (-1);
    }
    return (right_pid);
}

static int exec_pipeline(ast_node_t *node, t_env **env)
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
