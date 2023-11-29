#include "../minishell.h"

char	**cutstring(char *str, int position)
{
	char	**result;

	result = (char **)malloc(sizeof(char *) * 2);
	result[0] = (char *)malloc(sizeof(char) * (position + 1));
	result[1] = (char *)malloc(sizeof(char) * (ft_strlen(str) - position + 1));
	ft_strlcpy(result[0], str, position + 1);
	ft_strlcpy(result[1], str + position + 1, ft_strlen(str) - position + 1);
	return (result);
}

char	*get_name_env(int i, char **envcpy)
{
	int		n;
	char	*env;

	if (i == -1)
		return (NULL);
	n = 0;
	while (envcpy[i][n] != '=')
		n++;
	env = (char *)malloc(sizeof(char) * (n + 1));
	n = 0;
	while (envcpy[i][n] != '=')
	{
		env[n] = envcpy[i][n];
		n++;
	}
	env[n] = '\0';
	return (env);
}

void	cut_name_support(int *i, char *str, char *result)
{
	while (i[1] <= i[0])
	{
		result[i[2]] = str[i[1]];
		i[1]++;
		i[2]++;
	}
	result[i[2]] = '\0';
}

char	*cut_name(char *str, char *quotes, int j)
{
	char	*result;
	int		i[3];

	i[0] = 0;
	i[1] = 0;
	i[2] = 0;
	while ((str[i[0]] != '\0' && str[i[0]] != '$'
			&& ft_isalnum(str[i[0]]) == 0 && quotes[i[0]] == j)
		|| str[i[0]] == '_' || str[i[0]] == '?')
	{
		if (str[i[0]] == '?' && quotes[i[0]] != j)
			break ;
		i[0]++;
		i[1]++;
	}
	if (str[i[0]] == '\0')
		return ("");
	while (str[i[0]] != '\0')
		i[0]++;
	result = (char *)malloc(sizeof(char) * (i[0] - i[1] + 2));
	cut_name_support(i, str, result);
	return (result);
}

char	*get_env_val(int i, char **envcpy)
{
	int		n;
	int		j;
	char	*env;

	if (i == -1)
		return (NULL);
	n = 0;
	j = 0;
	while (envcpy[i][n] != '=')
		n++;
	env = (char *)malloc(sizeof(char) * (ft_strlen(envcpy[i]) - n + 1));
	n++;
	while (envcpy[i][n] != '\0')
	{
		env[j] = envcpy[i][n];
		n++;
		j++;
	}
	env[j] = '\0';
	return (env);
}

char	*ft_getenv(char *name, char **envcpy)
{
	int		i;
	char	*env;

	if (name[0] == '?')
		return (get_env_val(0, envcpy));
	i = find_env_line(name, envcpy);
	env = get_env_val(i, envcpy);
	return (env);
}

void	handle_free(t_dollar *dollar)
{
	free(dollar->halves[0]);
	free(dollar->halves[1]);
	free(dollar->halves);
	free(dollar->quotes[0]);
	free(dollar->quotes[1]);
	free(dollar->quotes);
	free(dollar->str);
	free(dollar->env);
}

char	*whats_name(char *str, char *quotes, int j)
{
	int		i;
	int		n;
	char	*name;

	i = 0;
	n = 0;
	while ((str[i] != '\0' && str[i] != '$'
			&& ft_isalnum(str[i]) == 0 && quotes[i] == j)
		|| str[i] == '_' || str[0] == '?')
	{
		if (str[0] == '?' && quotes[i] != j)
			break ;
		i++;
	}
	name = (char *)malloc(sizeof(char) * (i + 1));
	while (n < i)
	{
		name[n] = str[n];
		n++;
	}
	name[n] = '\0';
	return (name);
}

int	handle_support(t_dollar *dollar, t_node *head, t_ints ints)
{
	int		k;
	int		n;
	char	*tmpstr;
	char	*name;
	char	*tmp;

	n = head->quotes[ints.i][ints.j];
	dollar->halves = cutstring(head->args[ints.i], ints.n);
	dollar->quotes = cutstring(head->quotes[ints.i], ints.j);
	tmp = whats_name(dollar->halves[1], dollar->quotes[1], n);
	dollar->env = ft_getenv(tmp, dollar->envcpy);
	k = ft_strlen(dollar->env);
	free(tmp);
	if (dollar->env)
		dollar->str = ft_strjoin(dollar->halves[0], dollar->env);
	else
		dollar->str = ft_strjoin(dollar->halves[0], "");
	name = cut_name(dollar->halves[1], dollar->quotes[1], n);
	tmpstr = ft_strjoin(dollar->str, name);
	free(head->args[ints.i]);
	head->args[ints.i] = tmpstr;
	if (*name != '\0')
		free(name);
	return (k);
}

void	handle_support_2(t_dollar *dollar, t_node *tmp, int i, int n)
{
	t_ints	ints;

	ints.j = 0;
	ints.n = n;
	ints.i = i;
	while (tmp->args[ints.i][++ints.n])
	{
		if (tmp->args[ints.i][ints.n] == '$'
			&& tmp->quotes[ints.i][ints.j] != '1'
			&& tmp->args[ints.i][ints.n + 1] != '$')
		{
			if (tmp->args[ints.i][ints.n + 1] == '\0')
				break ;
			ints.n += handle_support(dollar, tmp, ints);
			ints.n -= 1;
			handle_free(dollar);
		}
		ints.j++;
	}
	return ;
}

void	handle_dollar(t_node **head, char ***envcpy)
{
	int			i;
	int			n;
	t_node		*tmp;
	t_dollar	*dollar;

	dollar = (t_dollar *)malloc(sizeof(t_dollar));
	dollar->envcpy = (*envcpy);
	tmp = *head;
	n = -1;
	while (tmp)
	{
		i = -1;
		while (tmp->args[++i])
			handle_support_2(dollar, tmp, i, n);
		tmp = tmp->next;
	}
	free(dollar);
}

int	ft_strncmp(char *s1, char *s2, int n)
{
	int	i;

	i = 0;
	if (!s1)
		return (1);
	while (s1[i] && s2[i] && i < n)
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	if (i == n)
		return (0);
	return (s1[i] - s2[i]);
}

int	len_til_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '=')
		i++;
	return (i);
}

int	ft_isalnum_dif(int c)
{
	if ((c >= '0' && c <= '9') || ft_isalpha(c) == 0 || c == '_' || c == '?')
		return (0);
	return (1);
}

int	len_til_special(char *str)
{
	int	i;

	i = 0;
	while (ft_isalnum_dif(str[i]) == 0)
		i++;
	return (i);
}

int	find_env_line(char *env, char **envcpy)
{
	int	i;
	int	n;
	int	len;

	i = 0;
	len = 0;
	while (envcpy[i] != NULL && i <= sizeof_array(envcpy))
	{
		if (ft_strncmp(envcpy[i], env, len_til_equal(envcpy[i])) == 0)
		{
			if (len_til_equal(envcpy[i]) > len
				&& len_til_equal(envcpy[i]) == len_til_special(env))
			{
				len = len_til_equal(envcpy[i]);
				n = i;
			}
		}
		i++;
	}
	if (len > 0)
		return (n);
	else
		return (-1);
}
