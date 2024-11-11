#include "minishell.h"

void	handle_eof(t_shell *shell)
{
	write(STDOUT_FILENO, "exit\n", 5);
	shell->exit_status = CTRL_D_EXIT_CODE;
	shell->running = false;
}

void signal_handler(int signum)
{
    if (signum == SIGINT)
    {
        g_exit_status = 130;  // 128 + SIGINT(2)
        write(STDERR_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void signal_handler_child(int signum)
{
    if (signum == SIGINT)
    {
        g_exit_status = 130;  // 128 + SIGINT(2)
        write(STDERR_FILENO, "\n", 1);
        exit(130);
    }
    else if (signum == SIGQUIT)
    {
        g_exit_status = 131;  // 128 + SIGQUIT(3)
        write(STDERR_FILENO, "Quit (core dumped)\n", 18);
        exit(131);
    }
}

/*void disable_ctrl_chars(void)
{
    struct termios term;

    if (tcgetattr(STDIN_FILENO, &term) == -1)
        return;
    term.c_lflag &= ~ECHOCTL;  // Disable control chars
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
        return;
}*/

/*void	disable_ctrl_chars(void)
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
}*/