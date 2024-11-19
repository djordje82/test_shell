/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:04:13 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 18:04:16 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_parent_only_builtin(char *cmd)
{
	return (ft_strncmp(cmd, "cd", 3) == 0 || ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0);
}

/*This function executes commands. It handles single commands, pipelines, and signals.*/
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
	setup_execution_signals(&sa_old_int, &sa_old_quit);
	if (current && !current->next && current->args
		&& is_builtin(current->args[0]))
		return (execute_single_builtin(current, shell));
	while (current)
	{
		if (!setup_pipeline_steps(current, prev_pipe, &last_pid, shell))
			return (1);
		current = current->next;
	}
	wait_for_children(last_pid);
	sigaction(SIGINT, &sa_old_int, NULL);
	sigaction(SIGQUIT, &sa_old_quit, NULL);
	return (g_exit_status);
}
