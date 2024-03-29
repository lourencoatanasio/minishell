#include "../minishell.h"

int	g_ec = 0;

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

char	**get_paths(char **envp)
{
	int		i;
	char	*aux;

	i = 0;
	while (envp[i])
	{
		if (envp[i][0] == 'P' && envp[i][1] == 'A'
			&& envp[i][2] == 'T' && envp[i][3] == 'H')
		{
			aux = ft_strnstr(envp[i], "PATH");
			if (aux)
				return (ft_split(aux, ':'));
		}
		i++;
	}
	return (NULL);
}

char	*find_path(char **paths, char *cmd)
{
	int		i;
	char	*path;
	char	*aux;

	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	i = 0;
	if (!paths)
		return (ft_strdup("not found"));
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
	path = ft_strdup("not found");
	return (path);
}

char	*ft_trim(char *str, char *set)
{
	int		i;
	int		j;
	char	*trim;

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
	return (trim);
}

int	get_quotes_support1(char *str, char *quotes, int *i)
{
	quotes[*i] = '0';
	if (str[*i] == '\'')
	{
		quotes[*i] = '3';
		(*i)++;
		while (str[*i] != '\'' && str[*i] != '\0')
		{
			quotes[*i] = '1';
			(*i)++;
		}
		if (str[*i] == '\0')
		{
			printf("Error: unclosed quotes\n");
			free(quotes);
			return (1);
		}
		else
			quotes[*i] = '3';
	}
	return (0);
}

int	get_quotes_support2(char *str, char *quotes, int *i)
{
	if (str[*i] == '\"')
	{
		quotes[*i] = '3';
		(*i)++;
		while (str[*i] != '\"' && str[*i] != '\0')
		{
			quotes[*i] = '2';
			(*i)++;
		}
		if (str[*i] == '\0')
		{
			printf("Error: unclosed quotes\n");
			free(quotes);
			return (1);
		}
		else
			quotes[*i] = '3';
	}
	return (0);
}

char	*get_quotes(char *str)
{
	int		i;
	char	*quotes;

	i = 0;
	if(!str)
		return (NULL);
	quotes = malloc(sizeof(char) * ft_strlen(str) + 10);
	while (str[i])
	{
		if (get_quotes_support1(str, quotes, &i) == 1)
			return (NULL);
		if (get_quotes_support2(str, quotes, &i) == 1)
			return (NULL);
		i++;
	}
	quotes[i] = '\0';
	return (quotes);
}

int	check_pipes_support(int pipes)
{
	if (pipes == 1)
	{
		printf("Error: empty pipe\n");
		g_ec = 2;
		return (1);
	}
	return (0);
}

int	check_pipes(char *line, char *quotes)
{
	int	i;
	int	pipes;

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
		else if (line[i] != ' ' && line[i] != '\t' && quotes[i] == '0')
			pipes = 0;
		i++;
	}
	if (check_pipes_support(pipes) == 1)
		return (1);
	return (0);
}

char	*store_cmds_quotes(char *line)
{
	char	*quotes;

	if (!line)
		return (NULL);
	quotes = get_quotes(line);
	if (!quotes)
	{
		g_ec = 2;
		return (NULL);
	}
	if (check_pipes(line, quotes) == 1)
	{
		free(quotes);
		return (NULL);
	}
	return (quotes);
}

char	***store_cmds(char *line)
{
	int		i;
	char	***cmds;
	char	**aux;
	char	*quotes;

	i = 0;
	quotes = store_cmds_quotes(line);
	if (!quotes)
		return (NULL);
	aux = ft_split_quote(line, quotes, '|');
	if (!aux)
		return (NULL);
	cmds = malloc(sizeof(char **) * (sizeof_array(aux) + 1));
	while (aux[i])
	{
		cmds[i] = ft_split_quote(aux[i], quotes, ' ');
		i++;
		free(quotes);
		quotes = get_quotes(aux[i]);
	}
	cmds[i] = NULL;
	free(quotes);
	free_array(aux);
	return (cmds);
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

int	sizeof_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

char	*remove_char_at_index(char *str, int index)
{
	int		len;
	int		i;
	int		j;
	char	*result;

	if (str == NULL || index < 0 || index >= ft_strlen(str))
		return (str);
	len = ft_strlen(str);
	result = (char *)malloc(len);
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
	return (result);
}

void	create_node_cmd_support(t_node **node, char **args, int i)
{
	if (sizeof_array(args) > 1)
	{
		while (args[i])
		{
			(*node)->args[i] = ft_strdup(args[i]);
			(*node)->quotes[i] = get_quotes((*node)->args[i]);
			i++;
		}
		(*node)->args[i] = NULL;
		(*node)->quotes[i] = NULL;
	}
	else
	{
		(*node)->args[0] = NULL;
		free((*node)->args[0]);
		(*node)->args[0] = ft_strdup((*node)->cmd);
		(*node)->quotes[0] = get_quotes((*node)->args[0]);
		(*node)->quotes[1] = NULL;
		(*node)->args[1] = NULL;
	}
}

t_node	*create_node_cmd(char **args, int error)
{
	int		i;
	t_node	*node;

	i = 0;
	node = (t_node *)malloc(sizeof(t_node));
	node->args = (char **)malloc(sizeof(char *) * (sizeof_array(args) + 1));
	node->quotes = (char **)malloc(sizeof(char *) * (sizeof_array(args) + 1));
	node->cmd = ft_strdup(args[0]);
	node->pipe = 0;
	create_node_cmd_support(&node, args, i);
	node->input = 0;
	node->output = 0;
	node->append = 0;
	node->here_doc = 0;
	node->next = NULL;
	node->error = error;
	return (node);
}

int	handle_quotes_support(t_node *tmp, int *i, int *j)
{
	if (tmp->quotes[*i] == NULL)
		return (1);
	if (tmp->quotes[*i][*j] == '3')
	{
		tmp->args[*i] = remove_char_at_index(tmp->args[*i], *j);
		tmp->quotes[*i] = remove_char_at_index(tmp->quotes[*i], *j);
		(*j)--;
	}
	return (0);
}

int	handle_quotes(t_node **head)
{
	int		i;
	int		j;
	t_node	*tmp;

	tmp = *head;
	while (tmp != NULL)
	{
		i = 0;
		while (tmp->args[i])
		{
			j = 0;
			while (tmp->args[i][j])
			{
				if (handle_quotes_support(tmp, &i, &j))
					return (1);
				j++;
			}
			i++;
		}
		tmp = tmp->next;
	}
	return (0);
}

t_node	*create_list(char ***cmds, t_node *head, int error)
{
	int	i;

	i = 0;
	head = NULL;
	if (!cmds)
		return (NULL);
	while (cmds[i])
	{
		add_node(&head, create_node_cmd(cmds[i], error));
		i++;
	}
	return (head);
}

void	print_list(t_node **head)
{
	int		i;
	t_node	*tmp;

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

int	is_builtin(char *str, char **args)
{
	if (!str)
		return (0);
	if (ft_strncmp(str, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(str, "cd", 3) == 0)
		return (2);
	if (ft_strncmp(str, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(str, "export", 7) == 0 && args[1])
		return (2);
	if (ft_strncmp(str, "export", 7) == 0 && !args[1])
		return (1);
	if (ft_strncmp(str, "unset", 6) == 0)
		return (2);
	if (ft_strncmp(str, "env", 4) == 0)
		return (1);
	if (ft_strncmp(str, "exit", 5) == 0)
		return (3);
	return (0);
}

void	builtin(char ***envpcpy, char **cmd, t_node **head)
{
	if (ft_strncmp(cmd[0], "echo", 5) == 0)
		shell_echo(head);
	else if (ft_strncmp(cmd[0], "cd", 3) == 0)
		shell_cd(head, (*envpcpy));
	else if (ft_strncmp(cmd[0], "pwd", 4) == 0)
		shell_pwd((*envpcpy), head);
	else if (ft_strncmp(cmd[0], "export", 7) == 0)
		shell_export(head, envpcpy);
	else if (ft_strncmp(cmd[0], "unset", 6) == 0)
		shell_unset(head, envpcpy);
	else if (ft_strncmp(cmd[0], "env", 4) == 0)
		shell_env((*envpcpy), head);
}

int	get_last_value(void)
{
	int		fd;
	int		r;
	char	*line;
	char	*tmp;

	r = 0;
	fd = open(".e", O_RDONLY);
	tmp = NULL;
	line = get_next_line(fd);
	while (line)
	{
		if (line && tmp)
			free(tmp);
		if (!line)
			break ;
		tmp = ft_strdup(line);
		free(line);
		line = get_next_line(fd);
	}
	if (tmp != NULL)
	{
		r = ft_atoi(tmp);
		free(tmp);
	}
	return (r);
}

int	node_count(t_node **head)
{
	int		i;
	t_node	*tmp;

	i = 0;
	tmp = *head;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

void	exit_status(t_node **head, int status, char **cmd)
{
	int		exit_status;
	char	*exit_status_str;

	if (is_builtin(cmd[0], (*head)->args) == 0 && WIFEXITED(status))
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

void	outputs(t_node **head, int *fd)
{
	if ((*head)->output == 0 && (*head)->append == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	else
	{
		close(fd[0]);
		if ((*head)->output != 0)
		{
			dup2((*head)->output, STDOUT_FILENO);
			close((*head)->output);
		}
		else if ((*head)->append != 0)
		{
			dup2((*head)->append, STDOUT_FILENO);
			close((*head)->append);
		}
		close(fd[1]);
	}
}

void	inputs(t_node **head, int *fd)
{
	(void)head;
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
}

int	child_one_support(t_child child, int fd[2])
{
	outputs(child.head, fd);
	if ((*child.head)->input != 0)
	{
		dup2((*child.head)->input, STDIN_FILENO);
		close((*child.head)->input);
	}
	else if ((*child.head)->here_doc != 0)
	{
		dup2((*child.head)->here_doc, STDIN_FILENO);
		close((*child.head)->here_doc);
	}
	close(fd[1]);
	if (is_builtin(child.cmd[0], (*child.head)->args) == 1
		|| is_builtin(child.cmd[0], (*child.head)->args) == 2)
		builtin(child.envp, child.cmd, child.head);
	else
		if (execve(child.path, child.cmd, (*child.envp)) == -1)
			return (1);
	return (0);
}

void	child_one_support_2(t_child child, int fd[2], pid_t pid)
{
	int	status;

	inputs(child.head, fd);
	waitpid(pid, &status, 0);
	exit_status(child.head, status, child.cmd);
}

int	child_one(char ***envp, char **cmd, char *path, t_node **head)
{
	int		fd[2];
	pid_t	pid;
	t_child	child;

	child.cmd = cmd;
	child.envp = envp;
	child.head = head;
	child.path = path;
	if (pipe(fd) == -1)
		return (1);
	pid = fork();
	sig_handler_block();
	if (pid == -1)
		return (1);
	else if (pid == 0)
	{
		if (child_one_support(child, fd))
			return (1);
		return (1);
	}
	else
		child_one_support_2(child, fd, pid);
	return (0);
}

void	execute_support(t_node **head)
{
	if ((*head)->input != 0 || (*head)->here_doc != 0)
	{
		if ((*head)->input != 0)
			dup2((*head)->input, STDIN_FILENO);
		else if ((*head)->here_doc != 0)
			dup2((*head)->here_doc, STDIN_FILENO);
	}
	if ((*head)->output != 0 || (*head)->append != 0)
	{
		if ((*head)->output != 0)
			dup2((*head)->output, STDOUT_FILENO);
		else if ((*head)->append != 0)
			dup2((*head)->append, STDOUT_FILENO);
	}
}

void execute_support1(char ***envpcpy, char **cmd, char *path, t_node **head)
{
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit(0);
	else if (pid == 0)
	{
		if (execve(path, cmd, (*envpcpy)) == -1)
			exit(0);
	}
	else
	{
		waitpid(pid, &status, 0);
		exit_status(head, status, cmd);
	}
}

int	execute(char ***envpcpy, char **cmd, char *path, t_node **head)
{
	if (((path != NULL && (access(path, F_OK) == 0))
			|| is_builtin(cmd[0], (*head)->args) != 0))
	{
		sig_handler_block();
		execute_support(head);
		if (is_builtin(cmd[0], (*head)->args) == 1)
			builtin(envpcpy, cmd, head);
		else
			execute_support1(envpcpy, cmd, path, head);
	}
	else
	{
		printf("minishell: %s: command not found\n", cmd[0]);
		g_ec = 127;
	}
	return (0);
}

int	pipex_support(t_pipex *p, char ***envpcpy, t_node **head)
{
	while (node_count(head) >= 2 && p->tmp->next != NULL)
	{
		p->cmd = (*p->tmp).args;
		p->path = find_path(p->paths, (*p->tmp).cmd);
		if (access(p->path, F_OK) == 0
			|| is_builtin(p->cmd[0], (*head)->args) != 0)
		{
			if (child_one(envpcpy, p->cmd, p->path, &p->tmp) == 1)
			{
				free(p->path);
				free_array(p->paths);
				return (1);
			}
		}
		else
		{
			printf("minishell: %s: command not found\n", (*p->tmp).cmd);
			g_ec = 127;
		}
		free(p->path);
		p->tmp = p->tmp->next;
	}
	return (0);
}

int ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	is_exit(t_pipex *p)
{
	printf("exit\n");
	free(p->path);
	free_array(p->paths);
	if (p->cmd[1])
	{
		if(ft_isdigit(p->cmd[1][0]) == 0)
			g_ec = 2;
		else
			g_ec = ft_atoi(p->cmd[1]);
	}
	return (1);
}

int	pipex_support_2(t_pipex *p, char ***envpcpy, t_node **head)
{
	p->cmd = (*p->tmp).args;
	p->path = find_path(p->paths, (*p->tmp).cmd);
	if (is_builtin(p->cmd[0], (*head)->args) != 0 || access(p->path, F_OK) == 0)
	{
		if (node_count(head) == 1 && is_builtin(p->cmd[0], (*head)->args) == 2)
			builtin(envpcpy, p->cmd, head);
		else if (node_count(head) == 1
				 && is_builtin(p->cmd[0], (*head)->args) == 3)
			return (is_exit(p));
		else
			execute(envpcpy, p->cmd, p->path, &p->tmp);
	}
	else
	{
		printf("minishell: %s: command not found\n", (*p->tmp).cmd);
		g_ec = 127;
	}
	return (0);
}

int	pipex(char ***envpcpy, t_node **head)
{
	t_pipex	p;

	if (!(*head))
		return (0);
	p.stdout = dup(STDOUT_FILENO);
	p.stdin = dup(STDIN_FILENO);
	p.paths = get_paths((*envpcpy));
	p.tmp = *head;
	if (pipex_support(&p, envpcpy, head))
		return (1);
	if(pipex_support_2(&p, envpcpy, head))
		return (1);
	wait(NULL);
	dup2(p.stdout, STDOUT_FILENO);
	dup2(p.stdin, STDIN_FILENO);
	free(p.path);
	free_array(p.paths);
	return (0);
}

char	**copy_env(char **envp)
{
	int		i;
	char	**envcpy;

	i = 0;
	while (envp[i])
		i++;
	envcpy = (char **)malloc(sizeof(char *) * (i + 2));
	i = 1;
	envcpy[0] = ft_strdup("?=0");
	while (envp[i])
	{
		envcpy[i] = ft_strdup(envp[i]);
		i++;
	}
	envcpy[i] = NULL;
	return (envcpy);
}

int	create_error_file(void)
{
	int		file;
	char	*path;
	char	tmp[1024];

	getcwd(tmp, sizeof(tmp));
	path = ft_strjoin(tmp, "/.e");
	file = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (file == -1)
	{
		printf("Error creating error file\n");
		exit(0);
	}
	free(path);
	return (file);
}

void	set_cmd(t_node **head)
{
	t_node	*tmp;

	tmp = *head;
	while (tmp)
	{
		free(tmp->cmd);
		tmp->cmd = ft_strdup(tmp->args[0]);
		tmp = tmp->next;
	}
}

int	check_only_spaces(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

void main_free(t_node **head, char ***cmds, char *line, char *written)
{
	free_list(head);
	free_triple(cmds);
	free(line);
	free(written);
}

int	main(int argc, char **argv, char **envp)
{
	int		error;
	char	*line;
	char	*written;
	char	*tmp;
	char	***cmds;
	char	**envcpy;
	char	*errado;
	t_node	*headmaster;

	(void )argc;
	(void )argv;
	envcpy = copy_env(envp);
	error = create_error_file();
	write(error, "0\n", 2);
	while (1)
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
		change_error(&envcpy, get_last_value());
		free(tmp);
		free(errado);
		if (!line)
		{
			printf("exit\n");
			free(written);
			free(line);
			break ;
		}
		if (line != NULL && ft_strlen(line) > 0 && check_only_spaces(line) == 0)
		{
			add_history(line);
			cmds = store_cmds(line);
			headmaster = create_list(cmds, headmaster, error);
			if (handle_quotes(&headmaster) == 0
				&& handle_redirections(&headmaster) == 0)
			{
				handle_dollar(&headmaster, &envcpy);
				set_cmd(&headmaster);
				if (pipex(&envcpy, &headmaster) != 0)
				{
					main_free(&headmaster, cmds, line, written);
					break ;
				}
			}
			main_free(&headmaster, cmds, line, written);
		}
		change_error(&envcpy, get_last_value());
	}
	rl_clear_history();
	free_array(envcpy);
	return (g_ec);
}
