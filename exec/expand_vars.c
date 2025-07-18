#include "../minishell.h"
#include <stdlib.h>
#include <string.h>

int	expand_var(const char *input, size_t i, size_t len,
	char *result, int ri, t_env *env, int last_status, size_t *out_i)
{
	if (i + 1 < len && input[i + 1] == '?')
	{
		ri += handle_dollar_question(result, ri, last_status);
		*out_i = i + 1;
	}
	else if (i + 1 < len && input[i + 1] == '{')
		ri = handle_brace_var(input, i, len, result, ri, env, out_i);
	else if (i + 1 < len && is_var_char(input[i + 1])
		&& (input[i + 1] < '0' || input[i + 1] > '9'))
		ri = handle_simple_var(input, i, len, result, ri, env, out_i);
	else
		result[ri++] = input[i];
	return (ri);
}

int	ensure_result_capacity(char **result, size_t *result_size, size_t ri)
{
	if (ri + 256 > *result_size)
	{
		size_t	old_size = *result_size;
		char	*new_result;

		*result_size *= 2;
		new_result = ft_realloc(*result, old_size, *result_size);
		if (!new_result)
		{
			free(*result);
			*result = NULL;
			return (0);
		}
		*result = new_result;
	}
	return (1);
}

void	update_quote_state(const char *input, size_t i, int *in_single, int *in_double)
{
	if (input[i] == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (input[i] == '"' && !(*in_single))
		*in_double = !(*in_double);
}

char	*expand_vars(const char *input, t_env *env, int last_status)
{
	size_t	len;
	size_t	result_size;
	size_t	ri;
	size_t	i;
	int		in_single;
	int		in_double;
	char	*result;

	if (!input)
		return (NULL);
	len = ft_strlen(input);
	result_size = len * 2 + 1;
	result = malloc(result_size);
	if (!result)
		return (NULL);
	ri = 0;
	in_single = 0;
	in_double = 0;
	i = 0;
	while (i < len)
	{
		if (!ensure_result_capacity(&result, &result_size, ri))
			return (NULL);
		update_quote_state(input, i, &in_single, &in_double);
		if (input[i] == '$' && !in_single)
			ri = expand_var(input, i, len, result, ri, env, last_status, &i);
		else if (!(input[i] == '\'' && !in_double) && !(input[i] == '"' && !in_single))
			result[ri++] = input[i];
		i++;
	}
	result[ri] = 0;
	return (result);
}
