/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:05:32 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/17 12:13:03 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_exit_status = 130;
		write(STDERR_FILENO, "\n", 1);
	}
}

int	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = heredoc_signal_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	return (1);
}

void	wait_for_children(pid_t last_pid)
{
	int		status;
	pid_t	wpid;
	bool	nl_printed;

	nl_printed = false;
	while (1)
	{
		wpid = wait(&status);
		if (wpid == -1)
			break ;
		if (WIFSIGNALED(status))
		{
			g_exit_status = 1300;
			if (WTERMSIG(status) == SIGQUIT)
				write(STDERR_FILENO, "Quit (core dumped)\n", 18);
			else if (WTERMSIG(status) == SIGINT && !nl_printed)
			{
				write(STDERR_FILENO, "\n", 1);
				nl_printed = true;
			}
		}
		else if (WIFEXITED(status))
		{
			if (wpid == last_pid)
				g_exit_status = WEXITSTATUS(status);
		}
	}
}

void	handle_wait_status(int status)
{
	if (WIFSIGNALED(status))
	{
		g_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 18);
		else if (WTERMSIG(status) == SIGINT)
			write(STDERR_FILENO, "\n", 1);
	}
	else if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
}
