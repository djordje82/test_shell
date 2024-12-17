/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/12/17 12:39:52 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

static void heredoc_handler(int signum)
{
	if (signum == SIGINT)
	{
	
		write(STDERR_FILENO, "\n", 1);
		exit (130);
	}
	else if (signum == SIGQUIT)
	{
		write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		exit(131);
	}
}	

int	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = heredoc_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (1);
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		return (1);
	return (0);
		
}

void restore_default_signals(void)
{
	struct sigaction sa;

	ft_memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	wait_for_children(pid_t last_pid)
{
	int		status;
	pid_t	wpid;
	int		signal;

	while (1)
	{
		wpid = wait(&status);
		if (wpid == -1)
			break ;
		if (WIFSIGNALED(status))
		{
			signal = WTERMSIG(status);
			if (signal == SIGQUIT)
				write(STDERR_FILENO, "Quit (core dumped)\n", 19);
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
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		else if (WTERMSIG(status) == SIGINT)
			write(STDERR_FILENO, "\n", 1);
	}
	else if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
}


/* void	wait_for_children(pid_t last_pid)
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
			g_exit_status = 0;
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
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		else if (WTERMSIG(status) == SIGINT)
			write(STDERR_FILENO, "\n", 1);
	}
	else if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
} */
