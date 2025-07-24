#include "../minishell.h"
#include <stdio.h>

const char	*get_env_value(const char *key, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
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
    char **result, size_t *result_size, int ri, t_env *env, size_t *out_i)
{
    size_t  j;
    size_t  start;
    char    *var;
    size_t  var_len;
    const char *value;
    size_t value_len;

    j = i + 1;
    start = j;
	var = malloc(sizeof(char) * (len - i + 1));
	if (!var)
		return (ri);
    while (j < len && is_var_char(input[j]))
        j++;
    var_len = j - start;
    ft_strncpy_safe(var, input + start, var_len);
    var[var_len] = 0;
    value = get_env_value(var, env);
    value_len = ft_strlen(value);
	free(var);
    if (!ensure_result_capacity(result, result_size, ri + value_len + 1))
        return ri;
    ri += ft_strcpy_to_buffer(*result + ri, value);
    *out_i = j;
    return ri;
}
