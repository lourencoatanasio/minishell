#include "../minishell.h"

int ft_atoi(char *str)
{
	int i;
	int sign;
	int num;

	i = 0;
	sign = 1;
	num = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' ||
				str[i] == '\v' || str[i] == '\f' || str[i] == '\r'))
		i++;
	if (str[i] && (str[i] == '-' || str[i] == '+'))
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
		num = num * 10 + (str[i++] - '0');
	return (num * sign);
}

void	print_array(char **array)
{
	int i;

	i = 0;
	while (array[i])
	{
        printf("=======================\narray check\n");
		printf("array[%d] = %s\n", i, array[i]);
		i++;
	}
	printf("=======================\n");
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
		if (access(path, F_OK) == 0)
		{
			printf("path = %s\n", path);
			return (path);
		}
		free(path);
		i++;
	}
	printf("Command not found\n");
	return (NULL);
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

char ***store_cmds(char *line)
{
	char ***cmds;
	char **aux;
	int i;

	i = 0;
	if(!line)
		return (NULL);
	cmds = malloc(sizeof(char **) * 10);
	aux = ft_split(line, '|');
	if(!aux) {
		free(cmds);
		return (NULL);
	}
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

int sizeof_array(char **array)
{
	int i;

	i = 0;
	while (array[i])
		i++;
	return i;
}

t_node	*create_node_cmd(char **args)
{
    t_node	*node;
	int		i;

	i = 0;
    node = (t_node *)malloc(sizeof(t_node));
    printf("sizeof_array = %d\n", sizeof_array(args));
	node->args = (char **)malloc(sizeof(char *) * sizeof_array(args));
    node->cmd = ft_strdup(args[0]);
	if(sizeof_array(args) > 1)
	{
		while(args[i])
		{
			node->args[i] = ft_strdup(args[i]);
			i++;
		}
		printf("check node\n");
		node->args[i] = NULL;
	}
	else
	{
		node->args[0] = node->cmd;
		node->args[1] = NULL;
	}
    node->input = 0;
    node->output = 0;
    node->append = 0;
    node->here_doc = 0;
    node->next = NULL;
    return (node);
}

t_node *create_list(char ***cmds, t_node *head)
{
	int i;

	i = 0;
	head = NULL;
	if(!cmds)
		return NULL;
	while (cmds[i])
	{
		add_node(&head, create_node_cmd(cmds[i]));
		i++;
	}
	return head;
}

void    print_list(t_node **head)
{
    t_node	*tmp;
    int		i;

	tmp = *head;
    while (tmp != NULL)
    {
        i = 0;
        printf("print_list_cmd = %s\n", tmp->cmd);
        while (tmp->args[i])
        {
            printf("print_list_args %d = %s\n", i, tmp->args[i]);
            i++;
        }
		printf("tmp = %p\n", tmp);
        tmp = tmp->next;
    }
}

void child_one(char **envp, char **cmd, char *path)
{
	int fd[2];
	pid_t pid;

	if (pipe(fd) == -1)
		exit(0);

	pid = fork();
	if (pid == -1)
		exit(0);
	else if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);  // Close the duplicated file descriptor
		execve(path, cmd, envp);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);  // Close the duplicated file descriptor
		printf("parent\n");
	}
}

int	node_count(t_node **head)
{
	t_node *tmp;
	int i;

	i = 0;
	tmp = *head;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return i;
}

void pipex(char **envp, t_node **head)
{
	char **paths;
	char *path;
	char **cmd;
	t_node *tmp;
	int stdout;
	int stdin;

	if (!(*head))
		return ;
	stdout = dup(STDOUT_FILENO);
	stdin = dup(STDIN_FILENO);
	paths = get_paths(envp);

	tmp = *head;
	while(node_count(head) >= 2 && tmp->next != NULL)
	{
		cmd = (*tmp).args;
		path = find_path(paths, (*tmp).cmd);
		child_one(envp, cmd, path);
		free(path);
		tmp = tmp->next;
	}

	cmd = (*tmp).args;
	path = find_path(paths, (*tmp).cmd);
	print_array(cmd);
	dup2(stdout, STDOUT_FILENO);
	if(fork() == 0)
		execve(path, cmd, envp);
	wait(NULL);
	dup2(stdin, STDIN_FILENO);
	free(path);
	free_array(paths);
	return ;
}

int main(int argc, char **argv, char **envp)
{
	char *line;
	char ***cmds;
    t_node *headmaster;

	(void )argc;
	(void )argv;
	while(1)
	{
		cmds = NULL;
		headmaster = NULL;
		line = NULL;
		sig_handler();
		line = readline("minishell$ ");
		if(!line)
		{
			free(line);
			exit(0);
		}
        if(line != NULL && ft_strlen(line) > 0)
        {
			add_history(line);
			cmds = store_cmds(line);
			if(!cmds)
				continue ;
//            print_array(cmds[0]);
            headmaster = create_list(cmds, headmaster);
			print_list(&headmaster);
            pipex(envp, &headmaster);
			free_list(&headmaster);
			print_triple(cmds);
            free_triple(cmds);
            free(line);
        }
	}
//	pipex(argc, argv, envp);
	return 0;
}
