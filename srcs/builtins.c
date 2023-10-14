#include "../minishell.h"

void shell_echo(t_node **head)
{
	int i = 1;
	int break_flag = 0;

	if ((*head)->args[1] && (*head)->args[1][0] == '-')
	{
		if ((*head)->args[1][1] == 'n' && (*head)->args[1][2] == '\0')
		{
			i++;
			break_flag = 1;
		}

	}
	while ((*head)->args[i])
	{
		printf("%s", (*head)->args[i]);
		if ((*head)->args[i + 1])
			printf(" ");
		i++;
	}
	if (break_flag == 0)
		printf("\n");
}

int	ft_strstr(char *str, char t_find)
{
	int i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == t_find)
			return (1);
		i++;
	}
	return (0);
}

void	shell_env(t_node **head, char **envpcpy)
{
	int i;

	i = 0;
	while (envpcpy[i])
	{
		if (ft_strstr(envpcpy[i], '=') == 1)
			printf("%s\n", envpcpy[i]);
		i++;
	}
    write((* head)->error, "0\n", 2);
}

void	shell_pwd(t_node **head, char **envpcpy)
{
	char *cwd;

	cwd = ft_getenv("PWD", envpcpy);
	printf("%s\n", cwd);
	free(cwd);
    write((* head)->error, "0\n", 2);
}

void ft_setenv(char *name, char *value, char **envpcpy)
{
	int i;
	char *aux;

	i = 0;
	while (envpcpy[i])
	{
		if (ft_strncmp(envpcpy[i], name, ft_strlen(name)) == 0)
		{
			aux = ft_strjoin(name, "=");
			aux = ft_strjoin(aux, value);
			free(envpcpy[i]);
			envpcpy[i] = ft_strdup(aux);
			free(aux);
            return ;
		}
		i++;
	}
	if(value != NULL)
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

char* ft_strchr(const char* str, int ch)
{
	while (*str != '\0')
	{
		if (*str == ch)
		{
			return (char*)str;
		}
		str++;
	}
	return NULL;
}

char **cpy_array(char **envpcpy)
{
	int i;
	char **aux;

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
	int i;
	int j;
	char *aux;

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

void print_ext_set(char **envpcpy)
{
	int i;
	char **envsorted;

	i = 0;
	envsorted = cpy_array(envpcpy);
	sort_array(envsorted);
	while (envsorted[i])
	{
		if (ft_strstr(envsorted[i], '=') == 1)
		{
			printf("declare -x %s=\"%s\"\n", get_name_env(i, envsorted), get_env_val(i, envsorted));
		}
		else
			printf("declare -x %s\n", envsorted[i]);
		i++;
	}
}

int ft_isalpha(int c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return (0);
    return (1);
}

int ft_isalnum(int c)
{
    if ((c >= '0' && c <= '9') || ft_isalpha(c) == 0)
        return (0);
    return (1);
}

int export_checker(char *str)
{
    int i;

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

void	shell_export(t_node **node, char **envpcpy)
{
	int i;
	char *aux;

	i = 1;
	printf("export\n");
	if(!(*node)->args[i])
	{
		printf("export: not enough arguments\n");
		print_ext_set(envpcpy);
        write((*node)->error, "0\n", 1);
		return ;
	}
	while ((*node)->args[i])
	{
        if (export_checker((*node)->args[i]) == 1)
        {
            printf("export: '%s': not a valid identifier\n", (*node)->args[i]);
            write((*node)->error, "1\n", 2);
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
		i++;
	}
}

int ft_strcmp(const char *s1, const char *s2)
{
	int i;

	i = 0;
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
	if (chdir((* head)->args[1] + 2) != 0)
	{
		printf("minishell: %s: Invalid directory\n", (* head)->args[1]);
		write((* head)->error, "1\n", 2);
	}
	else
	{
		ft_setenv("OLDPWD", getenv("OLDPWD"), envpcpy);
		ft_setenv("PWD", getcwd(NULL, 0), envpcpy);
		write((* head)->error, "0\n", 2);
	}
}

void	shell_cd(t_node **head, char **envpcpy)
{
	printf("cd\n");
	if ((* head)->args && (* head)->args[1] && (* head)->args[2])
	{
		printf("minishell: cd: too many arguments\n");
		write((* head)->error, "1\n", 2);
	}
	else if (!(* head)->args[1] || ft_strcmp((* head)->args[1], "~") == 0)
	{
		if (chdir(getenv("HOME")) != 0)
		{
			printf("minishell: cd: HOME not set\n");
			write((* head)->error, "1\n", 2);
		}
		else
		{
			ft_setenv("OLDPWD", getenv("OLDPWD"), envpcpy);
			ft_setenv("PWD", getcwd(NULL, 0), envpcpy);
			write((* head)->error, "0\n", 2);
		}
	}
	else if ((* head)->args[1][0] == '~' && (* head)->args[1][1] == '/')
		til(head, envpcpy);
	else if (ft_strcmp((* head)->args[1], "-") == 0)
	{
		if (chdir(getenv("OLDPWD")) != 0)
		{
			printf("minishell: cd: OLDPWD not set\n");
			write((* head)->error, "1\n", 2);
		}
		else
		{
			ft_setenv("OLDPWD", getenv("OLDPWD"), envpcpy);
			ft_setenv("PWD", getcwd(NULL, 0), envpcpy);
			write((* head)->error, "0\n", 2);
		}
	}
	else if (chdir((* head)->args[1]) != 0)
	{
		printf("minishell: cd : %s: No such file or directory\n", (* head)->args[1]);
		write((* head)->error, "1\n", 2);
	}
	else
	{
		ft_setenv("OLDPWD", getenv("OLDPWD"), envpcpy);
		ft_setenv("PWD", getcwd(NULL, 0), envpcpy);
		write((* head)->error, "0\n", 2);
	}
}

void	shell_unset(t_node **node, char **envpcpy)
{
	int i;
	int j;
	int k;

	i = 1;
	printf("unset\n");
	while ((*node)->args[i])
	{
		j = 0;
		while (envpcpy[j])
		{
			if (ft_strncmp(envpcpy[j], (*node)->args[i], ft_strlen((*node)->args[i])) == 0)
			{
				printf("envpcpy[%d] = %s\n", j, envpcpy[j]);
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
    write((* node)->error, "0\n", 2);
}
