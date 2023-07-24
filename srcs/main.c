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
//	last thing here running direct path
	if (access(cmd, F_OK) == 0)
		return (cmd);
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
			return (path);
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
	node->args = (char **)malloc(sizeof(char *) * (sizeof_array(args) + 1));
    node->cmd = ft_strdup(args[0]);
	if(sizeof_array(args) > 1)
	{
		while(args[i])
		{
			node->args[i] = ft_strdup(args[i]);
            printf("====================================\n");
            printf("args[%d] = %s\n", i, args[i]);
            printf("====================================\n");
            i++;
		}
		node->args[i] = NULL;
	}
	else
	{
        node->args[0] = NULL;
        free(node->args[0]);
		node->args[0] = ft_strdup(node->cmd);
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
    printf("i = %d\n", i);
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
	}
	wait(NULL);
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
	dup2(stdout, STDOUT_FILENO);
	if(fork() == 0)
		execve(path, cmd, envp);
	wait(NULL);
	dup2(stdin, STDIN_FILENO);
	free(path);
	free_array(paths);
	return ;
}

char **copy_env(char **envp)
{
    char **envcpy;
    int i;

    i = 0;
    while(envp[i])
        i++;
    envcpy = (char **)malloc(sizeof(char *) * (i + 1));
    i = 0;
    while(envp[i])
    {
        envcpy[i] = ft_strdup(envp[i]);
        i++;
    }
    envcpy[i] = NULL;
    return envcpy;
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

char *get_env(int i, char **envcpy)
{
    int n;
    int c;
    int tmp;
    char *env;

    n = 0;
    c = 0;
    while(envcpy[i][n] != '=')
        n++;
    tmp = n + 1;
    while(envcpy[i][n])
    {
        c++;
        n++;
    }
    env = (char *)malloc(sizeof(char) * (c + 1));
    c = 0;
    while(envcpy[i][tmp] != '\0')
    {
        env[c] = envcpy[i][tmp];
        tmp++;
        c++;
    }
    env[c] = '\0';
    return env;
}

int find_env_line(char *env, char **envcpy)
{
    int i;
    int n;

    i = 0;
    while(envcpy[i])
    {
        n = 0;
        while(envcpy[i][n] != '=')
            n++;
        if(ft_strncmp(envcpy[i], env, n) == 0)
            return i;
        i++;
    }
    return -1;
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

char *cut_name(char *str, char *name)
{
    int i;
    int n;

    i = 0;
    n = 0;
    printf("check\n");
    while(str[i] == name[n])
    {
        i++;
        n++;
    }
    return (str + i);
}

char ***handle_dollar(char ***cmds, char **envcpy)
{
    int i;
    int n;
    int c;
    char **halves;
    char *name;

    i = 0;
    c = 0;
    while(cmds[i])
    {
        n = 0;
        while(cmds[i][n])
        {
            printf("checking for $ in %s\n", cmds[i][n]);
            while(cmds[i][n][c])
            {
                if(cmds[i][n][c] == '$')
                {
                    halves = cutString(cmds[i][n], c);
                    printf("halves[0] = %s\n", halves[0]);
                    printf("halves[1] = %s\n", halves[1]);
                    if(find_env_line(halves[1], envcpy) != -1)
                    {
                        printf("found env\n");
                        cmds[i][n] = ft_strjoin(halves[0], get_env(find_env_line(halves[1], envcpy), envcpy));
                    }
                    else
                    {
                        printf("env not found\n");
                        cmds[i][n] = ft_strjoin(halves[0], "");
                    }
                    name = get_name_env(find_env_line(halves[1], envcpy), envcpy);
                    printf("name = %s\n", name);
                    printf("halves[1] after = %s\n", halves[1]);
                    halves[1] = cut_name(halves[1], name); //Não fica com o segundo $ por isso não é recursivo
                    cmds[i][n] = ft_strjoin(cmds[i][n], halves[1]);
                }
                c++;
                printf("c = %d\n", c);
            }
            c = 0;
            n++;
        }
        i++;
    }
    return cmds;
}

int main(int argc, char **argv, char **envp)
{
	char *line;
	char ***cmds;
    char **envcpy;
    t_node *headmaster;

	(void )argc;
	(void )argv;
    envcpy = copy_env(envp); //not freed
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
            cmds = handle_dollar(cmds, envcpy);
            print_triple(cmds);
			if(!cmds)
				continue ;
            print_triple(cmds);
            headmaster = create_list(cmds, headmaster);
            pipex(envcpy, &headmaster);
			free_list(&headmaster);
            free_triple(cmds);
            free(line);
        }
	}
//	pipex(argc, argv, envp);
	return 0;
}
