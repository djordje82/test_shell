/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:04:48 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 18:04:51 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_pipeline(t_command *current, int *pipe_fd, t_shell *shell)
{
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (current->next && pipe(pipe_fd) == -1)
		return (exit_error("pipe failed", NULL, 1, shell));
	if (current->args && is_parent_only_builtin(current->args[0]))
		execute_builtin(current, shell);
	return (1);
}

static int	create_process(pid_t *pid, t_shell *shell)
{
	*pid = fork();
	if (*pid == -1)
		return (exit_error("fork failed", NULL, 1, shell));
	return (1);
}

int	handle_pipeline(t_command *current, int *prev_pipe, pid_t *last_pid,
		t_shell *shell)
{
	int pipe_fd[2];
	pid_t pid;

	if (!init_pipeline(current, pipe_fd, shell))
		return (0);
	if (!create_process(&pid, shell))
		return (0);
	if (pid == 0)
		handle_child_process(current, prev_pipe, pipe_fd, shell);
	if (!current->next)
		*last_pid = pid;
	handle_parent_process(prev_pipe, pipe_fd);
	return (1);
}