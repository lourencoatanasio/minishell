//
// Created by ldiogo on 6/14/23.
//

#ifndef MINISHELL_H
# define MINISHELL_H

# include<stdio.h>
# include<readline/readline.h>
# include<readline/history.h>
# include<signal.h>
# include<unistd.h>
# include<stdlib.h>
# include<string.h>
# include<sys/wait.h>
# include<fcntl.h>

typedef struct s_node
{
	char *cmd;
	char **args;
	int input;
	int output;
	int append;
	int here_doc;
	struct s_node	*next;
}		t_node;

void	child_one(char **envp, char **cmd, char *path);
void    print_list(t_node *head);
void    fromtritolst(t_node **head, char ***cmds);
void    print_list(t_node *head);
t_node *create_list(char ***cmds);
t_node *create_node(char *cmd, char **args);
void	add_node(t_node **head, t_node *node);
char ***store_cmds(char *line, char **envp);
char *ft_trim(char *str, char *set);
void	free_array(char **array);
char	*find_path(char **paths, char *cmd);
char *ft_strdup(char *str);
char *ft_strjoin(char *s1, char *s2);
char **get_paths(char **envp);
char	*ft_strnstr(const char *big, const char *little);
char	**ft_split(char *str, char c);
char	*get_word(char *str, char c);
int	get_num_words(char *str, char c);
int ft_strlen(char *str);
void print_triple(char ***array);
void	print_array(char **array);

#endif //MINISHELL_H
