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
	char **quotes;
	int input;
	int output;
	int append;
	int here_doc;
	struct s_node	*next;
}		t_node;

typedef struct s_exporttable
{
	char					*k;
	char					*value;
	struct s_exporttable	*next;
}				t_extab;

void print_array(char **array);
void print_triple(char ***array);
int ft_strlen(char *str);
int	get_num_words(char *str, char c);
char	*get_word(char *str, char c);
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
void child_one(char **envp, char **cmd, char *path, t_node **head);
void	sig_handler(void);
char **cutString(char *str, int position);
char *get_name_env(int i, char **envcpy);
char *cut_name(char *str);
void handle_dollar(t_node **head, char **envcpy);
char *ft_getenv(char *name, char **envcpy);
char *get_env_val(int i, char **envcpy);
int find_env_line(char *env, char **envcpy);
char **copy_env(char **envp);
int ft_strncmp(char *s1, char *s2, int n);
char *ft_strlcpy(char *dst, const char *src, long unsigned int dstsize);

void	shell_pwd(char **envpcpy);
void shell_echo(t_node **head);
void	shell_env(char **envpcpy);
void	shell_unset(t_node **node, char **envpcpy);

void ft_setenv(char *name, char *value, char **envpcpy);
void ft_chdir(char *path, char**envpcpy);
char* ft_strchr(const char* str, int ch);
void	shell_export(t_node **node, char **envpcpy);

int ft_strcmp(const char *s1, const char *s2);
void	shell_cd(t_node **head, char **envpcpy);
void	til(t_node **head, char **envpcpy);


int ft_strfcmp(char *s1, char *s2);
int	get_num_words_quote(char *str, char *quotes, char c);
char	*get_word_quote(char *str, char *quotes, char c, int n);
char **ft_split_quote(char *str, char *quotes, char c);
int	is_builtin(char *str);
void	builtin(char **envpcpy, char **cmd, t_node **head);





#endif //MINISHELL_H
