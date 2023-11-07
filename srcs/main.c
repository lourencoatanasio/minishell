#include "../minishell.h"

int g_ec = 0;

void	print_array(char **array)
{
	int i;

	i = 0;
	while (array[i])
	{
        printf("=======================\n");
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

char	*find_path(char **paths, char *cmd)
{
	int		i;
	char	*path;
	char	*aux;

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

char *get_quotes(char *str)
{
	int i;
	char *quotes;

	i = 0;
	quotes = malloc(sizeof(char) * ft_strlen(str) + 10);
	while (str[i])
	{
		quotes[i] = '0';
		if (str[i] == '\'')
		{
			quotes[i] = '3';
			i++;
			while (str[i] != '\'' && str[i] != '\0')
			{
				quotes[i] = '1';
				i++;
			}
			if (str[i] == '\0')
			{
				printf("Error: unclosed quotes\n");
				return (NULL);
			}
			else
				quotes[i] = '3';
		}
		if (str[i] == '\"')
		{
			quotes[i] = '3';
			i++;
			while (str[i] != '\"' && str[i] != '\0')
			{
				quotes[i] = '2';
				i++;
			}
			if (str[i] == '\0')
			{
				printf("Error: unclosed quotes\n");
				return (NULL);
			}
			else
				quotes[i] = '3';
		}
		i++;
	}
	quotes[i] = '\0';
	return quotes;
}

int check_pipes(char *line, char *quotes)
{
	int i;
	int pipes;

	i = 0;
	pipes = 0;
	while (line[i])
	{
		if (line[i] == '|' && quotes[i] == '0')
		{
			pipes++;
			if (pipes > 1)
			{
				printf("Error: empty pipe\n");
				g_ec = 2;
				return (1);
			}
		}
		else if(line[i] != ' ' && line[i] != '\t' && quotes[i] == '0')
			pipes = 0;
		i++;
	}
	if(pipes == 1)
	{
		printf("Error: empty pipe\n");
		g_ec = 2;
		return (1);
	}
	return 0;
}

char ***store_cmds(char *line)
{
	char ***cmds;
	char **aux;
	char *quotes;
	int i;

	i = 0;
	if(!line)
		return (NULL);
	quotes = get_quotes(line);
	if (check_pipes(line, quotes) == 1)
	{
		free(quotes);
		return (NULL);
	}
	aux = ft_split_quote(line, quotes, '|');
	if(!aux)
		return (NULL);
	cmds = malloc(sizeof(char **) * (sizeof_array(aux) + 1));
	while (aux[i])
	{
		cmds[i] = ft_split_quote(aux[i], quotes, ' ');
		i++;
	}
	cmds[i] = NULL;
	free(quotes);
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

char* removeCharAtIndex(char* str, int index)
{
	char *result;
	int len;
	int i;
	int j;

	if (str == NULL || index < 0 || index >= ft_strlen(str))
		return (str);
	len = ft_strlen(str);
	result = (char*)malloc(len);
	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		if (i != index)
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	free(str);
	return result;
}

t_node	*create_node_cmd(char **args, int error)
{
    t_node	*node;
	int		i;

	i = 0;
    node = (t_node *)malloc(sizeof(t_node));
	node->args = (char **)malloc(sizeof(char *) * (sizeof_array(args) + 1));
	node->quotes = (char **)malloc(sizeof(char *) * (sizeof_array(args) + 1));
    node->cmd = ft_strdup(args[0]);
	if(sizeof_array(args) > 1)
	{
		while(args[i])
		{
			node->args[i] = ft_strdup(args[i]);
			node->quotes[i] = get_quotes(node->args[i]);
            i++;
		}
		node->args[i] = NULL;
		node->quotes[i] = NULL;
	}
	else
	{
        node->args[0] = NULL;
        free(node->args[0]);
		node->args[0] = ft_strdup(node->cmd);
		node->quotes[0] = get_quotes(node->args[0]);
		node->quotes[1] = NULL;
		node->args[1] = NULL;
	}
    node->input = 0;
    node->output = 0;
    node->append = 0;
    node->here_doc = 0;
    node->next = NULL;
    node->error = error;
    return (node);
}

void	handle_quotes(t_node **head)
{
	t_node	*tmp;
	int		i;
	int		j;

	tmp = *head;
	while (tmp != NULL)
	{
		i = 0;
		while (tmp->args[i])
		{
			j = 0;
			while (tmp->args[i][j])
			{
				if (tmp->quotes[i][j] == '3')
				{
					tmp->args[i] = removeCharAtIndex(tmp->args[i], j);
					tmp->quotes[i] = removeCharAtIndex(tmp->quotes[i], j);
					j--;
				}
				j++;
			}
			i++;
		}
		tmp = tmp->next;
	}
}

t_node *create_list(char ***cmds, t_node *head, int error)
{
	int i;

	i = 0;
	head = NULL;
	if(!cmds)
		return NULL;
	while (cmds[i])
	{
		add_node(&head, create_node_cmd(cmds[i], error));
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
			printf("print_list_quote %d = %s\n", i, tmp->quotes[i]);
            printf("print_list_args %d = %s\n", i, tmp->args[i]);
            i++;
        }
        tmp = tmp->next;
    }
}

int	is_builtin(char *str)
{
	if (!str)
		return (0);
	if (ft_strncmp(str, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(str, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(str, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(str, "export", 7) == 0)
		return (1);
	if (ft_strncmp(str, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(str, "env", 4) == 0)
		return (1);
	return (0);
}

void	builtin(char **envpcpy, char **cmd, t_node **head)
{
	if (ft_strncmp(cmd[0], "echo", 5) == 0)
		shell_echo(head);
	else if (ft_strncmp(cmd[0], "cd", 3) == 0)
		shell_cd(head, envpcpy);
	else if (ft_strncmp(cmd[0], "pwd", 4) == 0)
		shell_pwd(envpcpy, head);
	else if (ft_strncmp(cmd[0], "export", 7) == 0)
		shell_export(head, envpcpy);
	else if (ft_strncmp(cmd[0], "unset", 6) == 0)
		shell_unset(head, envpcpy);
	else if (ft_strncmp(cmd[0], "env", 4) == 0)
		shell_env(envpcpy, head);
}

int child_one(char **envp, char **cmd, char *path, t_node **head)
{
    int fd[2];
    int status;
    char *exit_status_str;
    int exit_status;
    pid_t pid;

    if (pipe(fd) == -1)
        exit(0);

    pid = fork();
	sig_handler_block();
	if (pid == -1)
        exit(0);
    else if (pid == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        if (is_builtin(cmd[0]) == 1)
            builtin(envp, cmd, head);
        else
        {
            if (execve(path, cmd, envp) == -1)
                exit(0);
        }
        return (1);
    }
    else
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        waitpid(pid, &status, 0);// Wait for the child process to finish
        if (WIFEXITED(status))
        {
            exit_status = WEXITSTATUS(status);
            exit_status_str = ft_itoa(exit_status);
            if (exit_status == 0)
                write((*head)->error, "0\n", 2);
            else
            {
                write((*head)->error, exit_status_str, ft_strlen(exit_status_str));
                write((*head)->error, "\n", 1);
            }
            free(exit_status_str);
			g_ec = get_last_value();
		}
	}
    return 0;
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

int    execute(char **envpcpy, char **cmd, char *path, t_node *head)
{
    pid_t pid;
    int status;
    int exit_status;
    char *exit_status_str;

	if(((path != NULL && (access(path, F_OK) == 0)) || is_builtin(cmd[0]) == 1))
	{
		pid = fork();
		sig_handler_block();
		if (pid == -1)
			exit(0);
		else if (pid == 0)
		{
			if (is_builtin(cmd[0]) == 1)
				builtin(envpcpy, cmd, &head);
			else
			{
				if (execve(path, cmd, envpcpy) == -1)
					exit(0);
			}
			return (1);
		}
		else
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
			{
				exit_status = WEXITSTATUS(status);
				exit_status_str = ft_itoa(exit_status);
				if (exit_status == 0)
					write(head->error, "0\n", 2);
				else
				{
					write(head->error, exit_status_str, ft_strlen(exit_status_str));
					write(head->error, "\n", 1);
				}
				free(exit_status_str);
			}
			g_ec = get_last_value();
		}
	}
	else
	{
		printf("minishell: %s: command not found\n", cmd[0]);
		g_ec = 127;
	}
	return (0);
}

int get_last_value()
{
    int fd;
    char *line;
    char *tmp;
    int r;

    r = 0;
    fd = open(".e", O_RDONLY);
	tmp = NULL;
    while ((line = get_next_line(fd)))
    {
		if (line && tmp)
			free(tmp);
		if (!line)
			break ;
		tmp = ft_strdup(line);
		free(line);
    }
    if (tmp != NULL)
    {
        r = ft_atoi(tmp);
        free(tmp);
    }
    return (r);
}

int pipex(char **envpcpy, t_node **head)
{
	char **paths;
	char *path;
	char **cmd;
	t_node *tmp;
	int stdout;
	int stdin;

	if (!(*head))
		return 0;
	stdout = dup(STDOUT_FILENO);
	stdin = dup(STDIN_FILENO);
	paths = get_paths(envpcpy);
	tmp = *head;
	while(node_count(head) >= 2 && tmp->next != NULL)
	{
		cmd = (*tmp).args;
		path = find_path(paths, (*tmp).cmd);
        if(access(path, F_OK) == 0 || is_builtin(cmd[0]) == 1)
        {
            if (child_one(envpcpy, cmd, path, &tmp) == 1)
            {
                free(path);
                free_array(paths);
                return (1);
            }
        }
		else
		{
			printf("minishell: %s: command not found\n", (*tmp).cmd);
			g_ec = 127;
		}
		free(path);
		tmp = tmp->next;
	}
	cmd = (*tmp).args;
	path = find_path(paths, (*tmp).cmd);
	dup2(stdout, STDOUT_FILENO);
	if (execute(envpcpy, cmd, path, *head) == 1)
	{
		free(path);
		free_array(paths);
		return (1);
	}
    wait(NULL);
	dup2(stdin, STDIN_FILENO);
	free(path);
	free_array(paths);
	return 0;
}

char **copy_env(char **envp)
{
    char **envcpy;
    int i;

    i = 0;
    while(envp[i])
        i++;
    envcpy = (char **)malloc(sizeof(char *) * (i + 2));
    i = 1;
	envcpy[0] = ft_strdup("?=0");
    while(envp[i])
    {
        envcpy[i] = ft_strdup(envp[i]);
        i++;
    }
    envcpy[i] = NULL;
    return envcpy;
}

int create_error_file()
{
    int file;
    char *path;
    char tmp[1024];

    getcwd(tmp, sizeof(tmp));
    path = ft_strjoin(tmp, "/.e");
    file = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (file == -1)
    {
        printf("Error creating error file\n");
        exit(0);
    }
    free(path);
    return file;
}

int main(int argc, char **argv, char **envp)
{
	char *line;
	char *written;
	char *tmp;
	char ***cmds;
    char **envcpy;
    int error;
	char *errado;

    t_node *headmaster;


    (void )argc;
	(void )argv;
    envcpy = copy_env(envp);
    error = create_error_file();
	write(error, "0\n", 2);
    while(1)
	{
		cmds = NULL;
		headmaster = NULL;
		line = NULL;
		sig_handler();
		written = getcwd(NULL, 0);
		tmp = ft_strjoin(written, "$ ");
		line = readline(tmp);
		errado = ft_itoa(g_ec);
		write(error, errado, ft_strlen(errado));
		write(error, "\n", 1);
		change_error(envcpy, get_last_value());
		free(tmp);
		free(errado);
		if(!line)
		{
			//control D
            printf("exit\n");
			free(written);
			free(line);
			break;
		}
        if(line != NULL && ft_strlen(line) > 0)
        {
			add_history(line);
			cmds = store_cmds(line);
            headmaster = create_list(cmds, headmaster, error);
			handle_quotes(&headmaster);
            handle_dollar(&headmaster, envcpy);
			if(pipex(envcpy, &headmaster) == 1)
			{
				free_list(&headmaster);
				free_triple(cmds);
				free(line);
				free(written);
				break ;
			}
			free_list(&headmaster);
			free_triple(cmds);
			free(line);
			free(written);
        }
		change_error(envcpy, get_last_value());
	}
	free_array(envcpy);
	return 0;
}
