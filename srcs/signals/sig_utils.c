/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:05:32 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/15 12:58:09 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_signal_handler(int signum)
{
	fprintf(stderr, "DEBUG: Heredoc signal handler (PID: %d) received signal: %d\n", 
            getpid(), signum);
	if (signum == SIGINT)
	{
		g_exit_status = 130;
		write(STDERR_FILENO, "\n", 1);
		fprintf(stderr, "DEBUG: About to close STDERR_FILENO in heredoc handler\n");
		exit (130);
	}
	else if (signum == SIGQUIT)
	{
		g_exit_status = 131;
		write(STDERR_FILENO, "Quit: 3\n", 8);
	}
	fprintf(stderr, "DEBUG: Exiting heredoc signal handler\n");
}

int	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	fprintf(stderr, "DEBUG: Setting up heredoc signals for PID: %d\n", getpid());
	ft_memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = heredoc_signal_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        fprintf(stderr, "DEBUG: Failed to set SIGINT handler\n");
        return (0);
    }
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
    {
        fprintf(stderr, "DEBUG: Failed to set SIGQUIT handler\n");
        return (0);
    }
    fprintf(stderr, "DEBUG: Heredoc signal setup complete\n");
    return (1);
}

void	wait_for_children(pid_t pid)
{
	int	status;

	fprintf(stderr, "DEBUG: Waiting for child PID: %d\n", pid);
	if (pid == -1)
	{
		perror("Bad PID");
		g_exit_status = 1;
		return ;
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		fprintf(stderr, "DEBUG: waitpid failed for PID: %d\n", pid);
		perror("waitpid failed");
		g_exit_status = 1;
	}
	fprintf(stderr, "DEBUG: Child process %d exited with status: %d\n", pid, status);
	handle_wait_status(status);
	fprintf(stderr, "DEBUG: After handle_wait_status, g_exit_status: %d\n", g_exit_status);
}

void	handle_wait_status(int status)
{
	fprintf(stderr, "DEBUG: Processing wait status: %d\n", status);
	if (WIFSIGNALED(status))
	{
		fprintf(stderr, "DEBUG: Process terminated by signal: %d\n", WTERMSIG(status));
		g_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		else if (WTERMSIG(status) == SIGINT)
			write(STDERR_FILENO, "\n", 1);
	}
	else if (WIFEXITED(status))
	{
        fprintf(stderr, "DEBUG: Process exited normally with status: %d\n", WEXITSTATUS(status));
        g_exit_status = WEXITSTATUS(status);
    }
}
