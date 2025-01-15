/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:05:42 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/15 13:58:34 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_exit_status = 0;

void	interactive_signal_handler(int signum)
{
	fprintf(stderr, "DEBUG: Interactive signal handler (PID: %d) received signal: %d\n",
            getpid(), signum);
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
	fprintf(stderr, "DEBUG: Exiting signal handler\n");
}

void	setup_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = interactive_signal_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

/* void	setup_child_signal(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
} */

void setup_child_signal(void)
{
    struct sigaction sa;

    fprintf(stderr, "DEBUG: Setting up child signals (PID: %d)\n", getpid());
    
    ft_memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = SIG_DFL;  // Change to default signal handling
    sa.sa_flags = 0;  // No special flags for child
    sigemptyset(&sa.sa_mask);

    // Set both signals to default handling
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        fprintf(stderr, "DEBUG: Failed to set child SIGINT handler\n");
    }
    if (sigaction(SIGQUIT, &sa, NULL) == -1) {
        fprintf(stderr, "DEBUG: Failed to set child SIGQUIT handler\n");
    }

    fprintf(stderr, "DEBUG: Child signal setup complete\n");
}

/* void	setup_exec_signals(struct sigaction *sa_old_int,
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
} */

void setup_exec_signals(struct sigaction *sa_old_int,
        struct sigaction *sa_old_quit)
{
    struct sigaction sa;

    fprintf(stderr, "DEBUG: Setting up exec signals for PID: %d\n", getpid());
    
    // Save current handlers
    sigaction(SIGINT, NULL, sa_old_int);
    sigaction(SIGQUIT, NULL, sa_old_quit);
    fprintf(stderr, "DEBUG: Parent saved handlers - INT: %p, QUIT: %p\n",
            sa_old_int->sa_handler, sa_old_quit->sa_handler);
    
    // Setup new handlers
    ft_memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;
    
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

void restore_exec_signals(struct sigaction *sa_old_int, struct sigaction *sa_old_quit)
{
    fprintf(stderr, "DEBUG: Restoring exec signals for PID: %d\n", getpid());
    fprintf(stderr, "DEBUG: Restoring handlers - INT: %p, QUIT: %p\n",
            sa_old_int->sa_handler, sa_old_quit->sa_handler);
    
    sigaction(SIGINT, sa_old_int, NULL);
    sigaction(SIGQUIT, sa_old_quit, NULL);
}