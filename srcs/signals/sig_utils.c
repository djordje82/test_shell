/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:05:32 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/25 15:41:03 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	interactive_signal_handler(int signum)
{
	g_exit_status = 1;
	if (signum == SIGINT)
	{
		g_exit_status = 130; // 128 + SIGINT(2)
		write(STDERR_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
	{
		g_exit_status = 131; // 128 + SIGQUIT(3)
		write(STDERR_FILENO, "Quit (core dumped)\n", 18);
	}
}

void	handle_eof(t_shell *shell)
{
	write(STDOUT_FILENO, "exit\n", 5);
	shell->exit_status = CTRL_D_EXIT_CODE;
	shell->running = false;
}

void	wait_for_children(pid_t last_pid)
{
	int		status;
	pid_t	wpid;

	while (1)
	{
		wpid = wait(&status);
		if (wpid == -1)
			break ;
		if (WIFSIGNALED(status))
		{
			g_exit_status = 0; //+ WTERMSIG(status);
			if (WTERMSIG(status) == SIGQUIT)
				write(STDERR_FILENO, "Quit (core dumped)\n", 18);
			else if (WTERMSIG(status) == SIGINT)
				write(STDERR_FILENO, "\n", 1);
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
