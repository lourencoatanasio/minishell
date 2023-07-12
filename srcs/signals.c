#include "../minishell.h"

static void	ctrlc(int s)
{
	if (s == 2)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	sig_handler(void)
{
	signal(SIGINT, &ctrlc);
	signal(SIGQUIT, SIG_IGN);
}