#include "minishell.h"

void	print_array(char **array)
{
	int i;

	i = 0;
	while (array[i])
	{
		printf("array[%d] = %s\n", i, array[i]);
		i++;
	}
}

void print_triple(char ***array)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (array[i])
	{
		while (array[i][j])
		{
			printf("array[%d][%d] = %s\n", i, j, array[i][j]);
			j++;
		}
		j = 0;
		i++;
	}
}

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

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
		if (envp[i][0] == 'P' && envp[i][1] == 'A' && envp[i][2] == 'T' && envp[i][3] == 'H')
		{
			aux = ft_strnstr(envp[i], "PATH");
			if (aux)
				return (ft_split(aux, ':'));
		}
        i++;
    }
    return NULL;
}

char *ft_strjoin(char *s1, char *s2)
{
	int i;
	int j;
	char *str;

	i = 0;
	j = 0;
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	return str;
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

char	*find_path(char **paths, char *cmd)
{
	int		i;
	char	*path;
	char	*aux;

	i = 0;
	aux = paths[0];
	paths[0] = ft_strdup(aux + 5);
	free(aux);
	while (paths[i])
	{
		aux = ft_strjoin(paths[i], "/");
		path = ft_strjoin(aux, cmd);
		free(aux);
		printf("path = %s\n", path);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	printf("Command not found\n");
	return (NULL);
}

void	free_array(char **array)
{
	int i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char *ft_trim(char *str, char *set)
{
	int i;
	int j;
	char *trim;

	i = 0;
	j = 0;
	trim = malloc(sizeof(char) * ft_strlen(str) + 1);
	while (str[i])
	{
		if (str[i] != set[0] && str[i] != set[1])
		{
			trim[j] = str[i];
			j++;
		}
		i++;
	}
	trim[j] = '\0';
	return trim;
}

char ***store_cmds(char *line, char **envp)
{
	char ***cmds;
	char **aux;
	int i;
	int j;

	i = 0;
	j = 0;
	cmds = malloc(sizeof(char **) * 10);
	aux = ft_split(line, '|');
	while (aux[i])
	{
		cmds[i] = ft_split(aux[i], ' ');
		i++;
	}
	cmds[i] = NULL;
	free_array(aux);
	return cmds;
}

void	add_node(t_node **head, t_node *node)
{
    t_node	*tmp;

    if (*head == NULL)
    {
        *head = node;
        return ;
    }
    tmp = *head;
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = node;
}

t_node	*create_node(char *cmd, char **args)
{
    t_node	*node;

    node = (t_node *)malloc(sizeof(t_node));
    node->cmd = cmd;
    node->args = args;
    node->input = 0;
    node->output = 0;
    node->append = 0;
    node->here_doc = 0;
    node->next = NULL;
    return (node);
}

t_node *create_list(char ***cmds)
{
	t_node *head;
	t_node *node;
	int i;

	i = 0;
	head = create_node(cmds[i][0], cmds[i]);
	i++;
	while (cmds[i])
	{
		node = create_node(cmds[i][0], cmds[i]);
		add_node(&head, node);
		i++;
	}
	return head;
}

void    fromtritolst(t_node **head, char ***cmds)
{
    t_node	*tmp;
    int		i;

    tmp = *head;
    i = 0;
    while (tmp != NULL)
    {
        tmp->cmd = **cmds;
        tmp->args = cmds[i];
        tmp = tmp->next;
        i++;
    }
}

void    print_list(t_node *head)
{
    t_node	*tmp;
    int		i;

    tmp = head;
    while (tmp != NULL)
    {
        i = 0;
        printf("cmd = %s\n", tmp->cmd);
        while (tmp->args[i])
        {
            printf("args %d = %s\n", i, tmp->args[i]);
            i++;
        }
        tmp = tmp->next;
    }
}

void	child_one(char **envp, char **cmd, char *path)
{
	int fd[2];
	char *line;
	pid_t pid;

	pid = fork();
	if (pipe(fd) == -1)
		exit(0);
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		execve(path, cmd, envp);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		printf("parent\n");
	}
}

//int pipex(int argc, char **argv, char **envp)
//{
//    char **paths;
//	char *path;
//	char **cmd;
//    int i = 0;
//
//    paths = 0;
//    paths = get_paths(envp);
//	path = find_path(paths, argv[1]);
//	cmd = clean_cm(argv, argc);
//	print_array(cmd);
//	while(cmd[i])
//	{
//		printf("cmd[%d] = %s\n", i, cmd[i]);
//		i++;
//	}
//	while(argc > 1)
//	{
//		child_one(envp, cmd, path);
//		argc--;
//	}
//	free(path);
//	free_array(paths);
//    return 0;
//}

int main(int argc, char **argv, char **envp)
{
    int i;
	char *line;
	char ***cmds;
    //double pointer á toa
    t_node **headmaster;

	line = NULL;
	while(1)
	{
		line = readline("minishell$ ");
		cmds = store_cmds(line, envp);
        headmaster[i] = create_list(cmds);
        print_list(headmaster[i]);
		//print_triple(cmds);
		free(line);
        i++;
	}
//	pipex(argc, argv, envp);
	return 0;
}
