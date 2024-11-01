#include "../../includes/minishell.h"

void	handle_eof(t_shell *shell)
{
	write(STDOUT_FILENO, "exit\n", 5);
	shell->exit_status = CTRL_D_EXIT_CODE;
	shell->running = false;
}

void	disable_ctrl_chars(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		return ;
}

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_exit_status = CTRL_C_EXIT_CODE;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	signal_handler_child(int signum)
{
	if (signum == SIGINT)
	{
		g_exit_status = CTRL_C_EXIT_CODE;
		write(STDOUT_FILENO, "\n", 1);
	}
	else if (signum == SIGQUIT)
	{
		g_exit_status = CTRL_BSLASH_EXIT_CODE;
		write(STDOUT_FILENO, "Quit (core dumped)\n", 18);
	}
}