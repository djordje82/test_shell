/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:04:13 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 17:21:52 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TO DO: SPLIT
int	execute_commands(t_shell *shell)
{
	t_command			*current;
	int					prev_pipe[2];
	struct sigaction	sa_old_int;
	struct sigaction	sa_old_quit;
	pid_t				last_pid;

	current = shell->cmnd_lst;
	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	last_pid = 0;
	if (current && !current->is_valid)
	{
		print_command_not_found(current);
		g_exit_status = 127;
		return (g_exit_status);
	}
	setup_execution_signals(&sa_old_int, &sa_old_quit);
	if (current && !current->next && current->args
		&& is_builtin(current->args[0]))
	{
		sigaction(SIGINT, &sa_old_int, NULL);
		sigaction(SIGQUIT, &sa_old_quit, NULL);
		return (execute_single_builtin(current, shell));
	}
	while (current)
	{
		if (!setup_pipeline_steps(current, prev_pipe, &last_pid, shell))
		{
			g_exit_status = 1;
			sigaction(SIGINT, &sa_old_int, NULL);
			sigaction(SIGQUIT, &sa_old_quit, NULL);
			return (g_exit_status);
		}
		current = current->next;
	}
	sigaction(SIGINT, &sa_old_int, NULL);
	sigaction(SIGQUIT, &sa_old_quit, NULL);
	wait_for_children(last_pid);
	return (g_exit_status);
}
