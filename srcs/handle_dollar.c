#include "../minishell.h"

char** cutString(char *str, int position)
{
	char** result = (char**)malloc(sizeof(char*) * 2);
	result[0] = (char*)malloc(sizeof(char) * (position + 1));
	result[1] = (char*)malloc(sizeof(char) * (ft_strlen(str) - position + 1));
	ft_strlcpy(result[0], str, position + 1);
	ft_strlcpy(result[1], str + position + 1, ft_strlen(str) - position + 1);
	return result;
}

char *get_name_env(int i, char **envcpy)
{
	int n;
	char *env;

	if(i == -1)
		return NULL;
	n = 0;
	while(envcpy[i][n] != '=')
		n++;
	env = (char *)malloc(sizeof(char) * (n + 1));
	n = 0;
	while(envcpy[i][n] != '=')
	{
		env[n] = envcpy[i][n];
		n++;
	}
	env[n] = '\0';
	return env;
}

char *cut_name(char *str)
{
	char *result;
	int i;
	int n;
	int c;

	i = 0;
	n = 0;
	c = 0;
	while((str[i] != '\0' && str[i] != '$' && ft_isalnum(str[i]) == 0) || str[i] == '_' || str[i] == '?')
    {
        i++;
        n++;
		if(str[i] == '?')
			break ;
    }
	if(str[i] == '\0')
		return "";
	while (str[i] != '\0')
		i++;
	result = (char *)malloc(sizeof(char) * (i - n + 2));
	while(n <= i)
	{
		result[c] = str[n];
		n++;
		c++;
	}
	result[c] = '\0';
	return (result);
}

char *get_env_val(int i, char **envcpy)
{
	int n;
	int j;
	char *env;

	if(i == -1)
		return NULL;
	n = 0;
	j = 0;
	while(envcpy[i][n] != '=')
		n++;
	env = (char *)malloc(sizeof(char) * (ft_strlen(envcpy[i]) - n + 1));
	n++;
	while(envcpy[i][n] != '\0')
	{
		env[j] = envcpy[i][n];
		n++;
		j++;
	}
	env[j] = '\0';
	return env;
}

char *ft_getenv(char *name, char **envcpy)
{
	int i;
	char *env;

	if(name[0] == '?')
		return get_env_val(0, envcpy);
	i = find_env_line(name, envcpy);
	env = get_env_val(i, envcpy);
	return env;
}

void handle_dollar(t_node **head, char **envcpy)
{
	int i;
	int n;
	t_node *tmp;
	char **halves;
	char *env;
	char *str;
	char *tmpstr;
	char *name;

	tmp = *head;
	while(tmp)
	{
		i = 0;
		while(tmp->args[i])
		{
			n = 0;
			while(tmp->args[i][n])
			{
				if(tmp->args[i][n] == '$' && tmp->quotes[i][n] != '1')
				{
					if(tmp->args[i][n + 1] == '\0')
						break;
					halves = cutString(tmp->args[i], n);
					env = ft_getenv(halves[1], envcpy);
					if(env)
                        str = ft_strjoin(halves[0], env);
					else
						str = ft_strjoin(halves[0], "");
					name = cut_name(halves[1]);
					tmpstr = ft_strjoin(str, name);
					free(tmp->args[i]);
					tmp->args[i] = tmpstr;
					if (*name != '\0')
						free(name);
					free(env);
					free(str);
					free(halves[1]);
					free(halves[0]);
					free(halves);
					n = -1;
				}
				n++;
			}
			i++;
		}
		tmp = tmp->next;
	}
}

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

int len_til_equal(char *str)
{
    int i;

    i = 0;
    while(str[i] != '=')
        i++;
    return i;
}

int ft_isalnum_dif(int c)
{
    if ((c >= '0' && c <= '9') || ft_isalpha(c) == 0 || c == '_' || c == '?')
        return (0);
    return (1);
}

int len_til_special(char *str)
{
    int i;

    i = 0;
    while(ft_isalnum_dif(str[i]) == 0)
        i++;
    return i;
}

int find_env_line(char *env, char **envcpy)
{
	int i;
    int n;
    int len;

	i = 0;
    len = 0;
	while(envcpy[i] != NULL && i <= sizeof_array(envcpy))
	{
		if(ft_strncmp(envcpy[i], env, len_til_equal(envcpy[i])) == 0)
        {
            if(len_til_equal(envcpy[i]) > len && len_til_equal(envcpy[i]) == len_til_special(env))
            {
                len = len_til_equal(envcpy[i]);
                n = i;
            }
        }
		i++;
	}
    if(len > 0)
        return n;
    else
	    return -1;
}