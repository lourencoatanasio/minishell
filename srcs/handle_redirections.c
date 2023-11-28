/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldiogo <ldiogo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 15:13:29 by ldiogo            #+#    #+#             */
/*   Updated: 2023/11/28 15:13:30 by ldiogo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strndup(char *str, int n)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = (char *)malloc(sizeof(char) * (n + 1));
	while (i < n)
	{
		tmp[i] = str[i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

char	*erase_redir(char *str)
{
	int		i;
	int		j;
	int		check;
	char	*tmp;

	i = 0;
	j = 0;
	check = 0;
	tmp = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	while (str[i] != '\0')
	{
		if ((str[i] == '>' || str[i] == '<') && check == 0)
		{
			i++;
			if (str[i] == '>' || str[i] == '<')
				i++;
			while (str[i] != '\0' && str[i] != '>' && str[i] != '<')
				i++;
			check = 1;
		}
		tmp[j] = str[i];
		j++;
	}
	tmp[j] = '\0';
	return (tmp);
}

char	*get_name_support(char *name)
{
	char	tmp[1024];
	char	*tmp2;
	char	*tmp3;
	char	*path;

	getcwd(tmp, sizeof(tmp));
	tmp2 = ft_strdup(name);
	tmp3 = ft_strjoin("/", tmp2);
	path = ft_strjoin(tmp, tmp3);
	free(tmp3);
	free(tmp2);
	return (path);
}

char	*get_name(char **args, int a, int i)
{
	int		j;
	char	name[100];
	char	*path;

	j = 0;
	if (!args)
		return (NULL);
	if (args[a][i] == '>' || args[a][i] == '<')
	{
		printf("minishell: syntax error near unexpected token `>'\n");
		return (NULL);
	}
	while (args[a][i] != '\0' && args[a][i] != '>' && args[a][i] != '<')
	{
		name[j] = args[a][i];
		args[a] = removeCharAtIndex(args[a], i);
		j++;
	}
	name[j] = '\0';
	path = get_name_support(name);
	return (path);
}

char	**remove_line_at_index(char **lines, int n)
{
	int		i;
	int		j;
	char	**tmp;

	i = 0;
	j = 0;
	while (lines[i])
		i++;
	tmp = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (lines[i])
	{
		if (i != n)
		{
			tmp[j] = ft_strdup(lines[i]);
			j++;
		}
		i++;
	}
	tmp[j] = NULL;
	free_array(lines);
	return (tmp);
}

char	*check_file_support(t_node *head, int i, int n)
{
	char	*tmp;
	char	**tmp2;
	char	**tmp3;

	tmp = get_name(head->args, i, n);
	if (head->args[i][0] == '\0')
	{
		tmp2 = cpy_array(head->args);
		tmp3 = cpy_array(head->quotes);
		free_array(head->args);
		free_array(head->quotes);
		head->args = remove_line_at_index(tmp2, i);
		head->quotes = remove_line_at_index(tmp3, i);
	}
	if (tmp == NULL)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (NULL);
	}
	return (tmp);
}

int	check_file_support4(void)
{
	printf("minishell: syntax error near unexpected token `newline'\n");
	return (1);
}

int	check_file_support2(t_node *head, int i, int n)
{
	if (head->args[i][n] == '>')
	{
		if (head->args[i][n + 1] == '>')
		{
			if (head->args[i][n + 2] == '>' || head->args[i][n + 2] == '<')
				return (check_file_support4());
			head->args[i] = removeCharAtIndex(head->args[i], n + 1);
			head->quotes[i] = removeCharAtIndex(head->quotes[i], n + 1);
		}
	}
	else if (head->args[i][n] == '<')
	{
		if (head->args[i][n + 1] == '<')
		{
			if (head->args[i][n + 2] == '>' || head->args[i][n + 2] == '<')
				return (check_file_support4());
			head->args[i] = removeCharAtIndex(head->args[i], n + 1);
			head->quotes[i] = removeCharAtIndex(head->quotes[i], n + 1);
		}
	}
	return (0);
}

void	check_file_support3(t_node *head, int i)
{
	char	**tmp2;
	char	**tmp3;

	tmp2 = cpy_array(head->args);
	tmp3 = cpy_array(head->quotes);
	free_array(head->args);
	free_array(head->quotes);
	head->args = remove_line_at_index(tmp2, i);
	head->quotes = remove_line_at_index(tmp3, i);
}

char	*check_file(t_node *head, int i, int n)
{
	if (check_file_support2(head, i, n) == 1)
		return (NULL);
	head->args[i] = removeCharAtIndex(head->args[i], n);
	head->quotes[i] = removeCharAtIndex(head->quotes[i], n);
	if (head->args[i][0] == '\0')
		check_file_support3(head, i);
	else if (head->args[i][n] == '\0')
	{
		n = 0;
		i++;
	}
	if (!head->args[i])
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (NULL);
	}
	if (head->args[i][n] != '\0')
		return (check_file_support(head, i, n));
	else
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (NULL);
	}
}

char	*check_name(char *path)
{
	int		i;
	char	*tmp;

	i = 0;
	while (path[i] != '\0')
		i++;
	i--;
	while (path[i] != '/')
		i--;
	i++;
	tmp = ft_strdup(&path[i]);
	return (tmp);
}

int	check_content(char *path)
{
	int		fd;
	char	*line;

	line = NULL;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (0);
	line = get_next_line(fd);
	while (line)
	{
		if (ft_strcmp(line, "") != 0)
		{
			close(fd);
			free(line);
			return (1);
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}

int	makeheredoc(char *limit)
{
	int		fd;
	char	*line;
	char	*tmp;

	if (check_content("./.heredoc") == 1)
		unlink("./.heredoc");
	fd = open("./.heredoc", O_RDWR | O_CREAT | O_APPEND, 0777);
	while (1)
	{
		write(1, "> ", 2);
		tmp = ft_strjoin(limit, "\n");
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

int	handle_redirections_support(t_node *tmp, int *i, int *n)
{
	char	*path;

	if (tmp->args[*i][*n + 1] == '>' && tmp->quotes[*i][*n] == '0')
	{
		path = check_file(tmp, *i, *n);
		if (path == NULL)
			return (1);
		tmp->append = open(path, O_WRONLY | O_CREAT | O_APPEND, 0777);
		tmp->output = 0;
	}
	else
	{
		path = check_file(tmp, *i, *n);
		if (path == NULL)
			return (1);
		tmp->output = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		tmp->append = 0;
	}
	free(path);
	*n = 0;
	*i = 0;
	if (tmp->args[*i] == NULL)
		return (2);
	return (0);
}

int	handle_redirections_support2(t_node *tmp, int *i, int *n)
{
	char	*path;

	if (tmp->args[*i][*n + 1] == '<' && tmp->quotes[*i][*n + 1] == '0')
	{
		path = check_file(tmp, *i, *n);
		if (path == NULL)
			return (1);
		makeheredoc(check_name(path));
		tmp->here_doc = open("./.heredoc", O_RDONLY, 0777);
		tmp->input = 0;
	}
	else
	{
		path = check_file(tmp, *i, *n);
		if (path == NULL)
			return (1);
		tmp->input = open(path, O_RDONLY);
		tmp->here_doc = 0;
	}
	free(path);
	*n = 0;
	*i = 0;
	if (tmp->args[*i] == NULL)
		return (2);
	return (0);
}

int	handle_redirections3(t_node *tmp, int *i, int *n)
{
	int	x;

	if (tmp->args[*i][*n] == '>' && tmp->quotes[*i][*n] == '0')
	{
		x = handle_redirections_support(tmp, i, n);
		if (x == 1)
			return (1);
		if (x == 2)
			return (2);
	}
	if (tmp->args[*i][*n] == '<' && tmp->quotes[*i][*n] == '0')
	{
		x = handle_redirections_support2(tmp, i, n);
		if (x == 1)
			return (1);
		if (x == 2)
			return (2);
	}
	return (0);
}

int	handle_redirections(t_node **head)
{
	t_node	*tmp;
	int		x;
	int		i;
	int		n;

	tmp = *head;
	while (tmp != NULL)
	{
		i = -1;
		while (tmp->args[++i])
		{
			n = -1;
			while (tmp->args[i][++n])
			{
				x = handle_redirections3(tmp, &i, &n);
				if (x == 1)
					return (1);
				if (x == 2)
					break ;
			}
		}
		tmp = tmp->next;
	}
	return (0);
}
