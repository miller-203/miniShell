#include "../minishell.h"

int expand_var(const char *input, size_t i, size_t len,
    char **result, size_t *result_size, int ri, t_env *env, int last_status, size_t *out_i)
{
    if (i + 1 < len && input[i + 1] == '?')
    {
        ri += handle_dollar_question(*result, ri, last_status);
        *out_i = i + 2;
    }
    else if (i + 1 < len && is_var_char(input[i + 1]))
        ri = handle_simple_var(input, i, len, result, result_size, ri, env, out_i);
    else
    {
        (*result)[ri++] = input[i];
        *out_i = i + 1;
    }
    return (ri);
}

int ensure_result_capacity(char **result, size_t *result_size, size_t needed_space)
{
    if (needed_space >= *result_size)
    {
        size_t old_size = *result_size;
        char *new_result;
        
        while (*result_size <= needed_space)
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

void update_quote_state(const char *input, size_t i, int *s_quote, int *d_quote)
{
    if (input[i] == '\'' && !(*d_quote))
        *s_quote = !(*s_quote);
    else if (input[i] == '"' && !(*s_quote))
        *d_quote = !(*d_quote);
}

char *expand_vars(const char *input, t_env *env, int last_status)
{
    size_t len;
    size_t result_size;
    size_t ri;
    size_t i;
    int s_quote;
    int d_quote;
    char *result;

    if (!input)
        return (NULL);
    len = ft_strlen(input);
    result_size = len + 1;
    result = malloc(result_size);
    if (!result)
        return (NULL);
    ri = 0;
    s_quote = 0;
    d_quote = 0;
    i = 0;
    while (i < len)
    {
        size_t estimated_need = ri;
        if (!ensure_result_capacity(&result, &result_size, estimated_need))
            return (NULL);
        update_quote_state(input, i, &s_quote, &d_quote);
        if (input[i] == '$' && !s_quote)
            ri = expand_var(input, i, len, &result, &result_size, ri, env, last_status, &i);
        else if (input[i] == '\'' && !d_quote)
            i++;
        else if (input[i] == '"' && !s_quote)
            i++;
        else
        {
            result[ri++] = input[i];
            i++;
        }
    }
    result[ri] = '\0';
    return (result);
}
