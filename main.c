#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	get_num_words(char *str, char c)
{
    int	i;
    int	words;

    i = 0;
    words = 0;
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
    array = 0;
    array = malloc(sizeof(char *) * words + 1);
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

char **get_paths(char **envp)
{
    char **paths;
    char *aux;
    int i;

    i = 0;
    while (envp[i])
    {
        //printf("%s\n", envp[i]);
        aux = ft_strnstr(envp[i], "PATH");
        if (aux)
            paths = ft_split(aux, ':');
        i++;
    }
    return paths;
}

int main(int argc, char **argv, char **envp)
{
    char **paths;
    int i = 0;

    paths = 0;
    paths = get_paths(envp);
    while (paths[i])
    {
        printf("%s\n", paths[i]);
        i++;
    }
    return 0;
}
