/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:05:42 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 18:05:44 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_exit_status;

void	setup_signals(void)
{
	struct sigaction	sa;

	// Initialize sigaction struct
	sa.sa_flags = 0; // Remove SA_RESTART;
	sigemptyset(&sa.sa_mask);
	// Interactive mode handler
	sa.sa_handler = signal_handler;
	sigaction(SIGINT, &sa, NULL);
	// Ignore SIGQUIT (Ctrl+\)
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
	// Disable ctrl character echoing
	// disable_ctrl_chars();
}

void	setup_execution_signals(struct sigaction *sa_old_int,
		struct sigaction *sa_old_quit)
{
	struct sigaction	sa;

	// Save old handlers
	sigaction(SIGINT, NULL, sa_old_int);
	sigaction(SIGQUIT, NULL, sa_old_quit);
	// Set parent process to ignore signals during command execution
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	setup_child_signals(void)
{
	struct sigaction sa;

	sa.sa_flags = 0; // Remove SA_RESTART for child processes
	sigemptyset(&sa.sa_mask);

	// Set default handlers for child processes
	sa.sa_handler = SIG_DFL; // Use default handler instead of custom
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}