#include "../minishell.h"

extern int	g_ec;

static void	ctrlc(int s)
{
	if (s == 2)
	{
		write(1, "\n", 1);
		g_ec = 130;
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

static void	ctrl_c_block(int sig)
{
	if (sig == 2)
	{
		write(1, "\n", 1);
		g_ec = 130;
		rl_on_new_line();
		rl_replace_line("", 0);
	}
	else
		g_ec = 0;
}

static void	ctrl_slash_block(int sig)
{
	(void) sig;
	write(1, "Quit (core dumped)\n", 19);
	g_ec = 131;
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	sig_handler_block(void)
{
	signal(SIGINT, &ctrl_c_block);
	signal(SIGQUIT, &ctrl_slash_block);
}
