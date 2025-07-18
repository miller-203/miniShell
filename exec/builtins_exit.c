#include "../minishell.h"

int	builtin_exit(void)
{
	exit(0);
	return (0);
}