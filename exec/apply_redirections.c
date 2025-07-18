#include "../minishell.h"
#include <unistd.h>
#include <fcntl.h>

static int	handle_redir_input(redirection_t *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd < 0)
	{
		print_exec_error("open input", NULL);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	handle_redir_output(redirection_t *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		print_exec_error("open output", NULL);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_redir_append(redirection_t *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		print_exec_error("open append", NULL);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_redir_heredoc(redirection_t *redir)
{
	int	fd;

	fd = heredoc(redir->filename);
	if (fd < 0)
	{
		print_exec_error("heredoc", "failed");
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	apply_redirections(redirection_t *redir)
{
	int	ret;

	while (redir)
	{
		ret = 0;
		if (redir->type == REDIR_INPUT)
			ret = handle_redir_input(redir);
		else if (redir->type == REDIR_OUTPUT)
			ret = handle_redir_output(redir);
		else if (redir->type == REDIR_APPEND)
			ret = handle_redir_append(redir);
		else if (redir->type == REDIR_HEREDOC)
			ret = handle_redir_heredoc(redir);
		if (ret < 0)
			return (-1);
		redir = redir->next;
	}
	return (0);
}
