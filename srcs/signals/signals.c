/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:05:42 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/09 13:24:23 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_exit_status = 0;

void	interactive_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_exit_status = 130;
		write(STDERR_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
	{
		g_exit_status = 131;
		write(STDERR_FILENO, "Quit (core dumped)\n", 18);
	}
}

void	setup_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = interactive_signal_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	setup_child_signal(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	setup_execution_signals(struct sigaction *sa_old_int,
		struct sigaction *sa_old_quit)
{
	struct sigaction	sa;

	sigaction(SIGINT, NULL, sa_old_int);
	sigaction(SIGQUIT, NULL, sa_old_quit);
	ft_memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	handle_eof(t_shell *shell)
{
	write(STDOUT_FILENO, "exit\n", 5);
	shell->exit_status = CTRL_D_EXIT_CODE;
	shell->running = false;
}
