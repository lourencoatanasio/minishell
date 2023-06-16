SRCS		=		main.c

OBJS 		=		$(addprefix objs/,$(SRCS:.c=.o))

CFLAGS		=		-g -Wall -Wextra -Werror
RM			=		rm -f

NAME		= 		minishell

objs/%.o: srcs/%.c
			@mkdir -p objs
#			@mkdir -p objs/builtins
#			@mkdir -p objs/exec
#			@mkdir -p objs/parser
#			@mkdir -p objs/utils
#			@mkdir -p objs/main
#			@mkdir -p objs/redirects
			@cc $(CFLAGS) -c $< -o $@

$(NAME):    	$(OBJS)
			@cc $(OBJS) $(CFLAGS) -o $(NAME) -lreadline

all:      	$(NAME)

clean:
			@$(RM) $(OBJS)

fclean:		clean
			@$(RM) $(NAME)

re:         fclean all