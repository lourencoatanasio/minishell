#include "../minishell.h"

char *ft_strndup(char *str, int n)
{
	int i;
	char *tmp;

	i = 0;
	tmp = (char *)malloc(sizeof(char) * (n + 1));
	while(i < n)
	{
		tmp[i] = str[i];
		i++;
	}
	tmp[i] = '\0';
	return tmp;
}

char *erase_redir(char *str)
{
	int i;
	int j;
	int check;
	char *tmp;

	i = 0;
	j = 0;
	check = 0;
	tmp = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	while(str[i] != '\0')
	{
		if((str[i] == '>' || str[i] == '<') && check == 0)
		{
			i++;
			if(str[i] == '>' || str[i] == '<')
				i++;
			while(str[i] != '\0' && str[i] != '>' && str[i] != '<')
				i++;
			check = 1;
		}
		tmp[j] = str[i];
		i++;
		j++;
	}
	tmp[j] = '\0';
	return tmp;
}

char *get_name(char **args, int a, int n)
{
	int i;
	int j;
	char tmp[1024];
	char name[100];
	char *tmp2;
	char *tmp3;
	char *path;

	i = n;
	j = 0;
	if(!args)
		return (NULL);
	if(args[a][i] == '>' || args[a][i] == '<')
	{
		printf("minishell: syntax error near unexpected token `>'\n");
		return (NULL);
	}
	while(args[a][i] != '\0' && args[a][i] != '>' && args[a][i] != '<')
	{
		name[j] = args[a][i];
		args[a] = removeCharAtIndex(args[a], i);
		j++;
	}
	name[j] = '\0';
	getcwd(tmp, sizeof(tmp));
	tmp2 = ft_strdup(name);
	tmp3 = ft_strjoin("/", tmp2);
	path = ft_strjoin(tmp, tmp3);
	free(tmp3);
	free(tmp2);
	return path;
}

char **removeLineAtIndex(char **lines, int n)
{
	int i;
	int j;
	char **tmp;

	i = 0;
	j = 0;
	while(lines[i])
		i++;
	tmp = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while(lines[i])
	{
		if(i != n)
		{
			tmp[j] = ft_strdup(lines[i]);
			j++;
		}
		i++;
	}
	tmp[j] = NULL;
	free_array(lines);
	return tmp;
}

char  *check_file(t_node *head, int i, int n)
{
	char *tmp;
	char **tmp2;
	char **tmp3;

	if (head->args[i][n] == '>')
	{
		if(head->args[i][n + 1] == '>')
		{
			if(head->args[i][n + 2] == '>' || head->args[i][n + 2] == '<') // means that theres 3 > or < in a row
			{
				printf("minishell: syntax error near unexpected token `newline'\n");
				return (NULL);
			}
			head->args[i] = removeCharAtIndex(head->args[i], n + 1);
			head->quotes[i] = removeCharAtIndex(head->args[i], n + 1);
		}
	}
	if (head->args[i][n] == '<')
	{
		if(head->args[i][n + 1] == '<')
		{
			if(head->args[i][n + 2] == '>' || head->args[i][n + 2] == '<') // means that theres 3 > or < in a row
			{
				printf("minishell: syntax error near unexpected token `newline'\n");
				return (NULL);
			}
			head->args[i] = removeCharAtIndex(head->args[i], n + 1);
			head->quotes[i] = removeCharAtIndex(head->args[i], n + 1);

		}
	}
	head->args[i] = removeCharAtIndex(head->args[i], n);
	head->quotes[i] = removeCharAtIndex(head->args[i], n);
	if(head->args[i][0] == '\0' && head->args[i + 1] != NULL)
	{
		tmp2 = cpy_array(head->args);
		tmp3 = cpy_array(head->quotes);
		free_array(head->args);
		free_array(head->quotes);
		head->args = removeLineAtIndex(tmp2, i);
		head->quotes = removeLineAtIndex(tmp3, i);
	}
	else
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (NULL);
	}
	if (head->args[i][n] != '\0') // means that the file name is in the same arg
	{
		tmp = get_name(head->args, i, n);
		if(head->args[i][0] == '\0')
		{
			tmp2 = cpy_array(head->args);
			tmp3 = cpy_array(head->quotes);
			free_array(head->args);
			free_array(head->quotes);
			head->args = removeLineAtIndex(tmp2, i);
			head->quotes = removeLineAtIndex(tmp3, i);
		}
		if (tmp == NULL)
		{
			printf("minishell: syntax error near unexpected token `newline'\n");
			return (NULL);
		}
		return (tmp);
	}
	return (NULL);
}

char *check_name(char *path)
{
	int i;
	char *tmp;

	i = 0;
	while(path[i] != '\0')
		i++;
	i--;
	while(path[i] != '/')
		i--;
	i++;
	tmp = ft_strdup(&path[i]);
	return tmp;
}

int check_content(char *path)
{
	int fd;
	char *line;

	line = NULL;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (0);
	while ((line = get_next_line(fd)))
	{
		if (ft_strcmp(line, "") != 0) // if the file is not empty
		{
			close(fd);
			free(line);
			return (1);
		}
		free(line);
	}
	close(fd);
	return (0); // if the file is empty
}

int	makeHereDoc(char *limit)
{
	int fd;
	char *line;

	printf("makeHereDoc\n");
	if(check_content("./.heredoc") == 1)
		unlink("./.heredoc");
	fd = open("./.heredoc", O_RDWR | O_CREAT | O_APPEND, 0777);
	while (1)
	{
		write(1, "> ", 2);
		char *tmp = ft_strjoin(limit, "\n");
		line = get_next_line(0);
		if (ft_strcmp(line, tmp) == 0)
		{
			free(tmp);
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
		free(tmp);
	}
	free(limit);
	return (fd);
}

int    handle_redirections(t_node **head)
{
	t_node *tmp;
	char *path;
	int i;
	int n;

	tmp = *head;
	print_list(&tmp);
	while (tmp != NULL)
	{
		i = 0;
		while (tmp->args[i])
		{
			n = 0;
			while (tmp->args[i][n])
			{
				if (tmp->args[i][n] == '>' && tmp->quotes[i][n] == '0')
				{
					printf("tmp->args[i] = %s\n", tmp->args[i]);
					printf("tmp->args[i][n] = %c\n", tmp->args[i][n]);
					printf("tmp->args[i][n + 1] = %c\n", tmp->args[i][n + 1]);
					printf("tmp->quotes[i][n] = %c\n", tmp->quotes[i][n]);
					printf("tmp->quotes[i][n + 1] = %c\n", tmp->quotes[i][n + 1]);
					printf("tmp->quotes[i] = %s\n", tmp->quotes[i]);
					printf("i = %d\n", i);
					if (tmp->args[i][n + 1] == '>' && tmp->quotes[i][n] == '0')
					{
						path = check_file(tmp, i, n);
						if (path == NULL)
							return (1);
						tmp->append = open(path, O_WRONLY | O_CREAT | O_APPEND, 0777);
						tmp->output = 0;
					}
					else
					{
						path = check_file(tmp, i, n);
						if (path == NULL)
							return (1);
						tmp->output = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
						tmp->append = 0;
					}
					free(path);
					n = 0;
					i = 0;
					if(tmp->args[i] == NULL)
						break;
				}
				if (tmp->args[i][n] == '<' && tmp->quotes[i][n] == '0')
				{
					if (tmp->args[i][n + 1] == '<' && tmp->quotes[i][n + 1] == '0')
					{
						path = check_file(tmp, i, n);
						printf("path = %s\n", path);
						if (path == NULL)
							return (1);
						tmp->here_doc = makeHereDoc(check_name(path));
						tmp->input = 0;
					}
					else
					{
						path = check_file(tmp, i, n);
						if (path == NULL)
							return (1);
						tmp->input = open(path, O_RDONLY);
						tmp->here_doc = 0;
					}
					free(path);
					n = 0;
					i = 0;
					if(tmp->args[i] == NULL)
						break;
				}
				n++;
			}
			i++;
		}
		tmp = tmp->next;
	}
	return (0);
}