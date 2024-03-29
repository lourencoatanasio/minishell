#include "../minishell.h"

size_t	ft_strlcpy_get(char *dst, char *src, size_t size)
{
	size_t	i;

	if (!dst || !src)
		return (0);
	else if (!size)
		return (ft_strlen(src));
	i = 0;
	while (src[i] && i < (size - 1))
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}

char	*ft_strnldup(char *s1)
{
	char	*str;
	size_t	i;

	i = 0;
	if (!s1)
		return (0);
	while (s1[i] && s1[i] != '\n')
		i++;
	if (s1[i] == '\n')
		str = (char *)malloc(sizeof(char) * (i + 2));
	else
		str = (char *)malloc(sizeof(char) * (i + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1, i + 2);
	if (str[0] == '\0')
	{
		free (str);
		return (NULL);
	}
	return (str);
}

char	*ft_exstrchr(char *s)
{
	int		i;
	char	*tmp;

	i = 0;
	while (s[i] != '\n' && s[i])
		i++;
	if (s[i] == '\0')
	{
		free(s);
		return (NULL);
	}
	tmp = malloc((ft_strlen(s) - i) + 1);
	if (!tmp)
		return (NULL);
	ft_strlcpy(tmp, s + i + 1, ft_strlen(s) - i + 1);
	free(s);
	return (tmp);
}

char	*get_nl(int fd, char *buffer, char *str, char *save)
{
	int		i;

	i = 1;
	while (!ft_strchr(str, '\n') && i)
	{
		i = read(fd, buffer, 1);
		if (i == -1)
			return (NULL);
		if (!str)
		{
			str = (char *)malloc(1);
			str[0] = '\0';
		}
		buffer[i] = '\0';
		save = str;
		str = ft_strjoin(save, buffer);
		free(save);
	}
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*str;
	char		*buffer;
	char		*save;

	if (read(fd, 0, 0) == -1)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	save = NULL;
	str = get_nl(fd, buffer, str, save);
	free(buffer);
	save = ft_strnldup(str);
	str = ft_exstrchr(str);
	return (save);
}
