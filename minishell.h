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

void print_array(char **array);
void print_triple(char ***array);
int ft_strlen(char *str);
int get_num_words(char *str, char c);
char *get_word(char *str, char c);
char **ft_split(char *str, char c);
char *ft_strnstr(const char *big, const char *little);
char **get_paths(char **envp);
char *ft_strjoin(char *s1, char *s2);
char *ft_strdup(char *str);
char *find_path(char **paths, char *cmd);
void free_array(char **array);
void free_list(t_node **head);
void free_triple(char ***triple);
char *ft_trim(char *str, char *set);
char ***store_cmds(char *line);
void add_node(t_node **head, t_node *node);
int sizeof_array(char **array);
t_node *create_node(char **args);
t_node *create_list(char ***cmds, t_node *head);
void print_list(t_node **head);
void child_one(char **envp, char **cmd, char *path);
void	sig_handler(void);
char **cutString(char *str, int position);
char *get_name_env(int i, char **envcpy);
char *cut_name(char *str, char *name);
void handle_dollar(t_node **head, char **envcpy);
char *get_env(int i, char **envcpy);
int find_env_line(char *env, char **envcpy);
char **copy_env(char **envp);
int ft_strncmp(char *s1, char *s2, int n);
char *ft_strlcpy(char *dst, const char *src, long unsigned int dstsize);


#endif //MINISHELL_H
