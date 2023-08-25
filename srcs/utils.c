#include "../minishell.h"

int ft_strncmp(char *s1, char *s2, int n)
{
	int i;

	i = 0;
	while(s1[i] && s2[i] && i < n)
	{
		if(s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	if(i == n)
		return 0;
	return (s1[i] - s2[i]);
}

char *ft_strlcpy(char *dst, const char *src, long unsigned int dstsize)
{
	size_t i;

	i = 0;
	if (dstsize == 0)
		return (dst);
	while (src[i] && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char *ft_strdup(char *str)
{
	int i;
	char *dup;

	i = 0;
	dup = malloc(sizeof(char) * ft_strlen(str) + 1);
	while (str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return dup;
}

char *ft_strjoin(char *s1, char *s2)
{
	int i;
	int j;
	char *str;

	i = 0;
	j = 0;
	if(!s2)
		return (s1);
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
	while (s1[i])
		str[j++] = s1[i++];
	i = 0;
	while (s2[i])
		str[j++] = s2[i++];
	str[j] = '\0';
	return (str);
}

char	*ft_strnstr(const char *big, const char *little)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (*little == '\0')
		return ((char *)big);
	while (big[i])
	{
		j = 0;
		while (little[j] == big[j + i])
		{
			j++;
			if (little[j] == '\0')
				return ((char *)(big + i));
		}
		i++;
	}
	return (NULL);
}