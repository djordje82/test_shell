/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/12/12 16:11:40 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

static int	execute_pipeline(t_command *current, t_shell *shell,
		struct sigaction *sa_old_int, struct sigaction *sa_old_quit)
{
	int		prev_pipe[2];
	pid_t	last_pid;

	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	last_pid = 0;
	while (current)
	{
		if (!setup_pipeline_steps(current, prev_pipe, &last_pid, shell))
		{
			g_exit_status = 1;
			sigaction(SIGINT, sa_old_int, NULL);
			sigaction(SIGQUIT, sa_old_quit, NULL);
			return (g_exit_status);
		}
		current = current->next;
	}
	wait_for_children(last_pid);
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
	setup_execution_signals(&sa_old_int, &sa_old_quit);
	if (current && !current->next && current->args
		&& is_builtin(current->args[0]))
	{
		sigaction(SIGINT, &sa_old_int, NULL);
		sigaction(SIGQUIT, &sa_old_quit, NULL);
		return (execute_single_builtin(current, shell));
	}
	return (execute_pipeline(current, shell, &sa_old_int, &sa_old_quit));
}
