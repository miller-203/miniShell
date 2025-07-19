#include "../minishell.h"

int	exec_pipeline_left(ast_node_t *node, t_env **env, int *pipefd)
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	exit(execute_ast(node->data.pipeline.left, env));
}

int	exec_pipeline_right(ast_node_t *node, t_env **env, int *pipefd)
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	exit(execute_ast(node->data.pipeline.right, env));
}

int	wait_pipeline_children(int left_pid, int right_pid)
{
	int	status1;
	int	status2;

	waitpid(left_pid, &status1, 0);
	waitpid(right_pid, &status2, 0);
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	return (1);
}

int fork_left_process(ast_node_t *node, t_env **env, int *pipefd)
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

int fork_right_process(ast_node_t *node, t_env **env, int *pipefd, int left_pid)
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
