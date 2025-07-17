#include "../minishell.h"

int	ft_itoa_to_buffer(char *buffer, int num)
{
	char	temp[32];
	int		i = 0;
	int		len = 0;
	int		is_negative = 0;
	int		n = num;

	if (num == 0)
	{
		buffer[0] = '0';
		buffer[1] = '\0';
		return (1);
	}
	if (n < 0)
	{
		is_negative = 1;
		n = -n;
	}
	while (n > 0)
	{
		temp[i++] = '0' + (n % 10);
		n /= 10;
	}
	if (is_negative)
		buffer[len++] = '-';
	while (i > 0)
		buffer[len++] = temp[--i];
	buffer[len] = '\0';
	return (len);
}

int	ft_strcpy_to_buffer(char *dest, const char *src)
{
	int	len;

	if (!src)
		src = "";
	len = 0;
	while (src[len])
	{
		dest[len] = src[len];
		len++;
	}
	dest[len] = '\0';
	return (len);
}

void	ft_strncpy_safe(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
}

char	*ft_realloc(char *ptr, size_t old_size, size_t new_size)
{
	char	*new_ptr;
	size_t	i;

	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	i = 0;
	if (ptr)
	{
		while (i < old_size && i < new_size)
		{
			new_ptr[i] = ptr[i];
			i++;
		}
		free(ptr);
	}
	return (new_ptr);
}
