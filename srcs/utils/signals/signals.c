#include "minishell.h"

int	g_exit_status;

void setup_signals(void)
{
    struct sigaction sa;
    
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
    //disable_ctrl_chars();
}

/*void setup_signals_heredoc(void)
{
    struct sigaction sa;
    
    sa.sa_flags = 0;  // No SA_RESTART for heredoc
    sigemptyset(&sa.sa_mask);
    
    sa.sa_handler = signal_handler_heredoc;
    sigaction(SIGINT, &sa, NULL);
    
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}*/

void handle_signals_child(void)
{
    struct sigaction sa;
    
    sa.sa_flags = 0; // Remove SA_RESTART for child processes
    sigemptyset(&sa.sa_mask);
    
    // Set default handlers for child processes
    sa.sa_handler = SIG_DFL;  // Use default handler instead of custom
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

/*void	setup_signals(void)
{
	disable_ctrl_chars();
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_signals_child(void)
{
	signal(SIGINT, signal_handler_child);
	signal(SIGQUIT, signal_handler_child);
}*/
