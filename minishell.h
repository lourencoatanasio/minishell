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
	struct t_node	*next;
}				t_node;

#endif //MINISHELL_H
