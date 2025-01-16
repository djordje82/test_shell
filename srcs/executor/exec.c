/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:32:53 by jadyar            #+#    #+#             */
/*   Updated: 2025/01/16 15:07:09 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_pipeline(t_command *current, t_shell *shell,
		struct sigaction *sa_old_int, struct sigaction *sa_old_quit)
{
	int				prev_pipe[2];
	pid_t			last_pid;
	struct termios	orig_termios;
	int				original_stdin_fd;

	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	last_pid = 0;
	if (isatty(STDIN_FILENO) && tcgetattr(STDIN_FILENO, &orig_termios) < 0)
		perror("tcgetattr");
	original_stdin_fd = dup(STDIN_FILENO);
	while (current)
	{
		if (!setup_pipeline_steps(current, prev_pipe, &last_pid, shell))
		{
			g_exit_status = 1;
			break ;
		}
		current = current->next;
	}
	wait_for_children(last_pid);
	if (isatty(STDIN_FILENO) && tcsetattr(STDIN_FILENO, 
			TCSANOW, &orig_termios) < 0)
		perror("tcsetattr");
	if (original_stdin_fd != -1)
	{
		if (dup2(original_stdin_fd, STDIN_FILENO) == -1)
			perror("dup2");
		close(original_stdin_fd);
	}
	sigaction(SIGINT, sa_old_int, NULL);
	sigaction(SIGQUIT, sa_old_quit, NULL);
	return (g_exit_status);
}

int	execute_commands(t_shell *shell)
{
	t_command			*current;
	struct sigaction	sa_old_int;
	struct sigaction	sa_old_quit;

	current = shell->cmnd_lst;
	if (current && !current->is_valid)
	{
		print_command_not_found(current);
		g_exit_status = 127;
		return (g_exit_status);
	}
	setup_exec_signals(&sa_old_int, &sa_old_quit);
	if (current && !current->next && current->args
		&& is_builtin(current->args[0]))
	{
		sigaction(SIGINT, &sa_old_int, NULL);
		sigaction(SIGQUIT, &sa_old_quit, NULL);
		return (execute_single_builtin(current, shell));
	}
	return (execute_pipeline(current, shell, &sa_old_int, &sa_old_quit));
}
