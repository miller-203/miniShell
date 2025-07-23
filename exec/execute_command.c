#include "../minishell.h"
static void	expand_command_args(command_t *cmd, t_env *env)
{
	int		i;
	char	*expanded;

	i = 0;
	while (i < cmd->arg_count)
	{
		expanded = expand_vars(cmd->args[i], env, 0);
		if (expanded)
		{
			free(cmd->args[i]);
			cmd->args[i] = expanded;
		}
		i++;
	}
}

static void	expand_redirections(redirection_t *redirections, t_env *env)
{
	redirection_t	*r;
	char			*expanded;

	r = redirections;
	while (r)
	{
		expanded = expand_vars(r->filename, env, 0);
		if (expanded)
		{
			free(r->filename);
			r->filename = expanded;
		}
		r = r->next;
	}
}

static int	execute_builtin_command(command_t *cmd, t_env **env)
{
	int	result;

	result = 1;
	if (ft_strcmp(cmd->name, "exit") == 0)
		result = builtin_exit();
	else if (ft_strcmp(cmd->name, "cd") == 0)
		result = builtin_cd(cmd->args, *env);
	else if (ft_strcmp(cmd->name, "pwd") == 0)
		result = builtin_pwd();
	else if (ft_strcmp(cmd->name, "echo") == 0)
		result = builtin_echo(cmd);
	else if (ft_strcmp(cmd->name, "env") == 0)
		result = builtin_env(*env);
	else if (ft_strcmp(cmd->name, "export") == 0)
		result = builtin_export(cmd->args, env);
	else if (ft_strcmp(cmd->name, "unset") == 0)
		result = builtin_unset(cmd->args, env);
	return (result);
}

static int	handle_builtin_execution(command_t *cmd, t_env **env)
{
	int	saved_stdin;
	int	saved_stdout;
	int	result;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (apply_redirections(cmd->redirections) < 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (1);
	}
	result = execute_builtin_command(cmd, env);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (result);
}

static void	execute_external_command(command_t *cmd, t_env *env)
{
	char	*exec_path;
	char	**envp;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (apply_redirections(cmd->redirections) < 0)
		exit(1);
	exec_path = find_in_path(cmd->name, env);
	if (!exec_path)
	{
		print_exec_error(cmd->name, "command not found");
		exit(127);
	}
	envp = env_list_to_environ(env);
	if (!envp)
	{
		print_exec_error("env", "failed to convert environment");
		free(exec_path);
		exit(1);
	}
	execve(exec_path, cmd->args, envp);
	print_exec_error("execve", NULL);
	free_environ(envp);
	free(exec_path);
	exit(127);
}

static int	fork_and_execute(command_t *cmd, t_env *env)
{
	pid_t	pid;
	int		status;
	char *expanded_name = expand_vars(cmd->name, env, 0);
    if (!expanded_name)
        return (1);
		free(cmd->name);
    cmd->name = expanded_name;
    expand_command_args(cmd, env);
	pid = fork();
	if (pid == 0)
		execute_external_command(cmd, env);
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (1);
	}
	print_exec_error("fork", NULL);
	return (1);
}

int	execute_command(command_t *cmd, t_env **env)
{
	int	i;

	if (!cmd || !cmd->name)
		return (1);
	expand_command_args(cmd, *env);
	expand_redirections(cmd->redirections, *env);
	i = 0;
	while (i < cmd->arg_count)
	{
		printf("cmd->args[%d]: %s\n", i, cmd->args[i]);
		i++;
	}
	if (is_builtin(cmd->name))
		return (handle_builtin_execution(cmd, env));
	return (fork_and_execute(cmd, *env));
}
