#include "../minishell.h"

const char	*get_env_value(const char *key, t_env *env)
{
	while (env)
	{
		if (strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return ("");
}

int	is_var_char(char c)
{
	return (c == '_' || (c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9'));
}

int	handle_dollar_question(char *result, int ri, int last_status)
{
	return (ft_itoa_to_buffer(result + ri, last_status));
}

int	handle_simple_var(const char *input, size_t i, size_t len,
	char *result, int ri, t_env *env, size_t *out_i)
{
	size_t	j;
	size_t	start;
	char	var[256];
	size_t	var_len;

	j = i + 1;
	start = j;
	while (j < len && is_var_char(input[j]))
		j++;
	var_len = j - start;
	if (var_len < sizeof(var))
	{
		ft_strncpy_safe(var, input + start, var_len);
		var[var_len] = 0;
		ri += ft_strcpy_to_buffer(result + ri, get_env_value(var, env));
	}
	*out_i = j;
	return (ri);
}
