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
# include<errno.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

typedef struct s_node
{
	char *cmd;
	char **args;
	char **quotes;
    int error;
	int input;
	int output;
	int append;
	int here_doc;
	int pipe;
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
t_node	*create_node_cmd(char **args, int error);
t_node *create_list(char ***cmds, t_node *head, int error);
void print_list(t_node **head);
int child_one(char **envp, char **cmd, char *path, t_node **head);
void	sig_handler(void);
void			sig_handler_block(void);
char	**cutstring(char *str, int position);
char *get_name_env(int i, char **envcpy);
char *cut_name(char *str);
void handle_dollar(t_node **head, char **envcpy);
char *ft_getenv(char *name, char **envcpy);
char *get_env_val(int i, char **envcpy);
int find_env_line(char *env, char **envcpy);
char **copy_env(char **envp);
int ft_strncmp(char *s1, char *s2, int n);
char *ft_strlcpy(char *dst, const char *src, long unsigned int dstsize);

void	shell_pwd(char **envpcpy, t_node **head);
void shell_echo(t_node **head);
void	shell_env(char **envpcpy, t_node **head);
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
int	is_builtin(char *str, char **args);
void	builtin(char **envpcpy, char **cmd, t_node **head);

int ft_isalnum(int c);
int ft_isalpha(int c);

void	change_error(char **envcpy, int value);
char    *ft_itoa(int n);
void    write_error(t_node **head);

char	*ft_strchr(const char *s, int c);
char	*get_nl(int fd, char *buffer, char *str, char *save);
char	*get_next_line(int fd);
char	*ft_exstrchr(char *s);
char	*ft_strnldup(char *s1);
int     ft_atoi(char *str);
size_t	ft_strlcpy_get(char *dst, char *src, size_t size);
int		get_last_value();

char *ft_strndup(char *str, int n);
char *get_name(char **args, int a, int n);
char  *check_file(t_node *head, int i, int n);
int    handle_redirections(t_node **head);
char* removeCharAtIndex(char* str, int index);
void	free_lines(char **array);
char **cpy_array(char **array);

#endif //MINISHELL_H
