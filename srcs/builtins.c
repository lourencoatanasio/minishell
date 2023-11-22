#include "../minishell.h"

extern int	g_ec;

char	*ft_itoa(int n)
{
	char	*str;
	int		i;
	int		aux;

	i = 0;
	aux = n;
	while (aux != 0)
	{
		aux = aux / 10;
		i++;
	}
	str = (char *)malloc(sizeof(char) * (i + 1));
	str[i] = '\0';
	i--;
	while (i >= 0)
	{
		str[i] = (n % 10) + '0';
		n = n / 10;
		i--;
	}
	return (str);
}

void	change_error(char **envcpy, int value)
{
	char	*aux;
	char	*aux2;

	free(envcpy[0]);
	if (value == 0)
		envcpy[0] = ft_strdup("?=0");
	else
	{
		aux2 = ft_itoa(value);
		aux = ft_strjoin("?=", aux2);
		envcpy[0] = ft_strdup(aux);
		free(aux);
		free(aux2);
	}
}

void	print_args(t_node **head, int i)
{
	while ((*head)->args[i])
	{
		printf("%s", (*head)->args[i]);
		if ((*head)->args[i + 1])
			printf(" ");
		i++;
	}
}

void	shell_echo(t_node **head)
{
	int	i;
	int	break_flag;

	i = 1;
	break_flag = 0;
	if ((*head)->args[1] && (*head)->args[1][0] == '-')
	{
		if ((*head)->args[1][1] == 'n' && (*head)->args[1][2] == '\0')
		{
			i++;
			break_flag = 1;
		}
	}
	print_args(head, i);
	if (break_flag == 0)
		printf("\n");
	write((*head)->error, "0\n", 2);
}

int	ft_strstr(char *str, char t_find)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == t_find)
			return (1);
		i++;
	}
	return (0);
}

void	shell_env(char **envpcpy, t_node **head)
{
	int	i;

	i = 1;
	(void) head;
	while (envpcpy[i])
	{
		if (ft_strstr(envpcpy[i], '=') == 1)
			printf("%s\n", envpcpy[i]);
		i++;
	}
	write((*head)->error, "0\n", 2);
}

void	shell_pwd(char **envpcpy, t_node **head)
{
	char	*cwd;

	(void) head;
	cwd = ft_getenv("PWD", envpcpy);
	printf("%s\n", cwd);
	free(cwd);
	write((*head)->error, "0\n", 2);
}

void	set_env_support(char *name, char *value, char **envpcpy, int i)
{
	char	*aux;

	aux = ft_strjoin(name, "=");
	aux = ft_strjoin(aux, value);
	free(envpcpy[i]);
	envpcpy[i] = ft_strdup(aux);
	free(aux);
	return ;
}

void	ft_setenv(char *name, char *value, char **envpcpy)
{
	int		i;
	char	*aux;

	i = -1;
	while (envpcpy[++i])
	{
		if (ft_strncmp(envpcpy[i], name, ft_strlen(name)) == 0)
		{
			set_env_support(name, value, envpcpy, i);
			return ;
		}
	}
	if (value != NULL)
	{
		aux = ft_strjoin(name, "=");
		aux = ft_strjoin(aux, value);
		envpcpy[i] = ft_strdup(aux);
		free(aux);
	}
	else
	{
		free(envpcpy[i]);
		envpcpy[i] = ft_strdup(name);
	}
	envpcpy[i + 1] = NULL;
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)(s + i));
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)(s + i));
	return (NULL);
}

char	**cpy_array(char **envpcpy)
{
	int		i;
	char	**aux;

	i = 0;
	while (envpcpy[i])
		i++;
	aux = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (envpcpy[i])
	{
		aux[i] = ft_strdup(envpcpy[i]);
		i++;
	}
	aux[i] = NULL;
	return (aux);
}

void	sort_array(char **envsorted)
{
	int		i;
	int		j;
	char	*aux;

	i = 0;
	while (envsorted[i])
	{
		j = i + 1;
		while (envsorted[j])
		{
			if (ft_strcmp(envsorted[i], envsorted[j]) > 0)
			{
				aux = envsorted[i];
				envsorted[i] = envsorted[j];
				envsorted[j] = aux;
			}
			j++;
		}
		i++;
	}
}

void	print_ext_set(char **envpcpy)
{
	int		i;
	char	**envsorted;

	i = 1;
	envsorted = cpy_array(envpcpy);
	sort_array(envsorted);
	while (envsorted[i])
	{
		if (ft_strstr(envsorted[i], '=') == 1)
		{
			printf("declare -x %s=\"%s\"\n", get_name_env(i, envsorted),
				get_env_val(i, envsorted));
		}
		else
			printf("declare -x %s\n", envsorted[i]);
		i++;
	}
}

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (0);
	return (1);
}

int	ft_isalnum(int c)
{
	if ((c >= '0' && c <= '9') || ft_isalpha(c) == 0)
		return (0);
	return (1);
}

int	export_checker(char *str)
{
	int	i;

	i = 0;
	if (ft_isalpha(str[i]) == 1)
		return (1);
	while (str[i] != '\0' && str[i] != '=')
	{
		if (ft_isalnum(str[i]) == 1 && str[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

void	shell_export_support(t_node **node, char **envpcpy, int i)
{
	char	*aux;

	if (export_checker((*node)->args[i]) == 1)
	{
		printf("export: '%s': not a valid identifier\n", (*node)->args[i]);
		write((*node)->error, "1\n", 2);
		return ;
	}
	else if (ft_strchr((*node)->args[i], '=') != NULL)
	{
		aux = ft_strchr((*node)->args[i], '=');
		*aux = '\0';
		aux++;
		ft_setenv((*node)->args[i], aux, envpcpy);
	}
	else
		ft_setenv((*node)->args[i], NULL, envpcpy);
}

void	shell_export(t_node **node, char **envpcpy)
{
	int	i;

	i = 1;
	if (!(*node)->args[i])
	{
		print_ext_set(envpcpy);
		write((*node)->error, "0\n", 2);
		return ;
	}
	while ((*node)->args[i])
	{
		shell_export_support(node, envpcpy, i);
		i++;
	}
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!s1)
		return (1);
	while (s1[i])
	{
		if ((s1[i] - s2[i]) != 0)
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

void	til(t_node **head, char **envpcpy)
{
	chdir(getenv("HOME"));
	if (chdir((*head)->args[1] + 2) != 0)
	{
		printf("minishell: %s: Invalid directory\n", (*head)->args[1]);
		write((*head)->error, "1\n", 2);
	}
	else
	{
		ft_setenv("OLDPWD", getenv("OLDPWD"), envpcpy);
		ft_setenv("PWD", getcwd(NULL, 0), envpcpy);
		write((*head)->error, "0\n", 2);
	}
}

void	shell_cd_support(t_node **head, char **envpcpy)
{
	if (chdir(getenv("HOME")) != 0)
	{
		printf("minishell: cd: HOME not set\n");
		write((*head)->error, "1\n", 2);
	}
	else
	{
		ft_setenv("OLDPWD", getenv("OLDPWD"), envpcpy);
		ft_setenv("PWD", getcwd(NULL, 0), envpcpy);
		write((*head)->error, "0\n", 2);
	}
}

void	shell_cd(t_node **head, char **envpcpy)
{
	if ((*head)->args && (*head)->args[1] && (*head)->args[2])
	{
		printf("minishell: cd: too many arguments\n");
		write((*head)->error, "1\n", 2);
	}
	else if (!(*head)->args[1] || ft_strcmp((*head)->args[1], "~") == 0)
		shell_cd_support(head, envpcpy);
	else if ((*head)->args[1][0] == '~' && (*head)->args[1][1] == '/')
		til(head, envpcpy);
	else if (chdir((*head)->args[1]) != 0)
	{
		printf("minishell: cd : %s: No such file or directory\n",
			(*head)->args[1]);
		write((*head)->error, "127\n", 4);
	}
	else
	{
		ft_setenv("OLDPWD", getenv("OLDPWD"), envpcpy);
		ft_setenv("PWD", getcwd(NULL, 0), envpcpy);
		write((*head)->error, "0\n", 2);
	}
}

void	shell_unset(t_node **node, char **envpcpy)
{
	int	i;
	int	j;
	int	k;

	i = 1;
	while ((*node)->args[i])
	{
		j = 0;
		while (envpcpy[j])
		{
			if (ft_strncmp(envpcpy[j],
					(*node)->args[i], ft_strlen((*node)->args[i])) == 0)
			{
				k = j;
				while (envpcpy[k])
				{
					envpcpy[k] = envpcpy[k + 1];
					k++;
				}
			}
			j++;
		}
		i++;
	}
	write((*node)->error, "0\n", 2);
}
