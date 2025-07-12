#include "../minishell.h"

static int	count_words(const char *s, char c)
{
	int	check;
	int		count;
	int		i;

	i = 0;
	count = 0;
	check = 1;
	while (s[i])
	{
		if (s[i] != c && check)
		{
			check = 0;
			count++;
		}
		else if (s[i] == c)
			check = 1;
		i++;
	}
	return (count);
}

static char	*ft_strncpy(char *dst, const char *src, size_t n)
{
	while (n-- && *src)
		*dst++ = *src++;
	*dst = '\0';
	return (dst);
}

static char	*get_word(const char **s, char c)
{
	const char	*start;
	size_t		len;
	char		*word;

	start = *s;
	while (**s && **s == c)
		(*s)++;
	start = *s;
	while (**s && **s != c)
		(*s)++;
	len = *s - start;
	word = (char *)malloc(len + 1);
	if (word == NULL)
		return (NULL);
	ft_strncpy(word, start, len);
	word[len] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	int			word_count;
	char		**array;
	int			i;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	array = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (array == NULL)
		return (NULL);
	i = 0;
	while (i < word_count)
	{
		array[i] = get_word(&s, c);
		if (!array[i])
		{
			while (i > 0)
				free(array[--i]);
			free(array);
			return (NULL);
		}
		i++;
	}
	array[word_count] = NULL;
	return (array);
}