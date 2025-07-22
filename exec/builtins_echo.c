#include "../minishell.h"

int	builtin_echo(command_t *cmd)
{
	int nflag = 0;
	int i = 1;
	int printed = 0;

	if (cmd->args[1] && ft_strcmp(cmd->args[1], "-n") == 0)
	{
		nflag = 1;
		i = 2;
	}
	while (i < cmd->arg_count)
	{
		if (cmd->args[i][0] != '\0' || cmd->was_quoted[i])
		{
			if (printed)
				printf(" ");
			printf("%s", cmd->args[i]);
			printed = 1;
		}
		i++;
	}
	if (!nflag)
		printf("\n");
	return (0);
}
