/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:05:42 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/25 17:06:51 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_exit_status = 0;

/* void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = interactive_signal_handler;
	sigaction(SIGINT, &sa, NULL);
	// Ignore SIGQUIT (Ctrl+\)
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	setup_child_signal(void)
{
	struct sigaction sa;

	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_DFL; 
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
} */

void	init_signals(bool is_parent)
{
	struct	sigaction sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (is_parent)
	{
		sa.sa_handler = interactive_signal_handler;
		sigaction(SIGINT, &sa, NULL);
		sa.sa_handler = SIG_IGN;
		sigaction(SIGQUIT, &sa, NULL);
	}
	else
	{
		sa.sa_handler = SIG_DFL;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
	}
}

void	setup_execution_signals(struct sigaction *sa_old_int,
		struct sigaction *sa_old_quit)
{
	struct sigaction	sa;

	sigaction(SIGINT, NULL, sa_old_int);
	sigaction(SIGQUIT, NULL, sa_old_quit);
	// Set parent process to ignore signals during command execution
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
