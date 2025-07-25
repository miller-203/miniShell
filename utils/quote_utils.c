#include "../minishell.h"

int	verify_quotes(const char *str)
{
	int	single;
	int	dbl;
	int	i;

	single = 0;
	dbl = 0;
	i = 0;
	while (str && str[i])
	{
		if (str[i] == '\'' && dbl == 0)
			single = !single;
		else if (str[i] == '"' && single == 0)
			dbl = !dbl;
		i++;
	}
	return (single == 0 && dbl == 0);
}
