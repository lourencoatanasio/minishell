#include "../minishell.h"

int get_num_words_quote(char* str, char* quotes, char c)
{
	int i;
	int words;

	i = 0;
	words = 0;
	if (!str || !quotes)
		return 0;
	while (str[i] && str[i] == c && quotes[i] != '0')
		i++;
	while (i < ft_strlen(str))
	{
		words++;
		while (i < ft_strlen(str) && !(str[i] == c && quotes[i] == '0'))
			i++;
		while (i < ft_strlen(str) && (str[i] == c && quotes[i] == '0'))
			i++;
	}
	return words;
}

char* get_word_quote(char* str, char* quotes, char c, int n)
{
	int i;
	int len;
	char* word;

	i = 0;
	if (!str || !quotes)
		return NULL;
	while (i < ft_strlen(str) && !(str[i] == c && quotes[n] == '0'))
	{
		n++;
		i++;
	}
	len = i;
	word = (char*)malloc(sizeof(char) * (len + 1));
	if (!word)
		return NULL;
	i = 0;
	while (i < len) {
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return word;
}

char	**ft_split_quote(char *str, char *quotes, char c)
{
	int		i;
	int		n;
	int		words;
	char	**array;

	i = 0;
	n = 0;
	words = get_num_words_quote(str, quotes, c);
	if(words == 0)
		return (NULL);
	array = malloc(sizeof(char *) * words + 10);
	if (!array)
		return (NULL);
	while (*str && *str == c)
	{
		n++;
		str++;
	}
	while (*str)
	{
		array[i] = get_word_quote(str, quotes, c, n);
		while (*str && !(*str == c && quotes[n] == '0'))
		{
			str++;
			n++;
		}
		while (*str && (*str == c && quotes[n] == '0'))
		{
			str++;
			n++;
		}
		i++;
	}
	array[i] = (char *) NULL;
	return (array);
}

int	get_num_words(char *str, char c)
{
	int	i;
	int	words;

	i = 0;
	words = 0;
	if(!str)
		return (0);
	while (str[i] && str[i] == c)
		i++;
	while (str[i])
	{
		words++;
		while (str[i] && str[i] != c)
			i++;
		while (str[i] && str[i] == c)
			i++;
	}
	return (words);
}

char	*get_word(char *str, char c)
{
	int		i;
	int		len;
	char	*word;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	len = i;
	word = malloc(sizeof(char) * len + 1);
	i = 0;
	while (i < len)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split(char *str, char c)
{
	int		i;
	int		words;
	char	**array;

	i = 0;
	words = get_num_words(str, c);
	if(words == 0)
		return (NULL);
	array = malloc(sizeof(char *) * words + 10);
	if (!array)
		return (NULL);
	while (*str && *str == c)
		str++;
	while (*str)
	{
		array[i] = get_word(str, c);
		while (*str && *str != c)
			str++;
		while (*str && *str == c)
			str++;
		i++;
	}
	array[i] = (char *) NULL;
	return (array);
}
