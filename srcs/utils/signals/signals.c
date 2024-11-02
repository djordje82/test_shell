#include "minishell.h"

int	g_exit_status;

void	setup_signals(void)
{
	disable_ctrl_chars();
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_signals_child(void)
{
	signal(SIGINT, signal_handler_child);
	signal(SIGQUIT, signal_handler_child);
}
