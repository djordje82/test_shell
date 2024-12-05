/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:04:48 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/05 14:41:01 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_pipeline(t_command *current, int *pipe_fd, t_shell *shell)
{
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (current->next && !create_pipe(pipe_fd, shell))
	{
		perror("pipe failed");
		return (0);
	}
	if (current->args && is_builtin(current->args[0]))
	{
		if (is_parent_only_builtin(current->args[0]) && !current->next
			&& !current->prev)
		{
			if (!setup_redirections(current))
				return (0);
			handle_builtin_cmd(current, shell);
			return (1);
		}
	}
	return (1);
}

static int	handle_invalid_command(t_command *current)
{
	if (!current->is_valid)
	{
		if (!current->prev)
			print_command_not_found(current);
		return (0);
	}
	return (1);
}

static void	handle_child_process(t_command *current, int *prev_pipe,
		int *pipe_fd, t_shell *shell)
{
	setup_child_signal();
	if (!current->is_valid)
	{
		cleanup_pipeline_resources(prev_pipe, pipe_fd);
		exit(127);
	}
	handle_pipeline_child(current, prev_pipe, pipe_fd, shell);
}

int	setup_pipeline_steps(t_command *current, int *prev_pipe, pid_t *last_pid,
		t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;

	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (!handle_invalid_command(current))
		return (0);
	signal(SIGINT, SIG_IGN);
	if (!init_pipeline(current, pipe_fd, shell))
		return (0);
	if (!create_process(&pid, shell))
	{
		cleanup_pipeline_resources(prev_pipe, pipe_fd);
		return (0);
	}
	if (pid == 0)
		handle_child_process(current, prev_pipe, pipe_fd, shell);
	if (!current->next)
		*last_pid = pid;
	handle_parent_process(prev_pipe, pipe_fd);
	return (1);
}

void	execute_pipeline_cmd(t_command *cmd, char *cmd_path, t_shell *shell)
{
	setup_child_signal();
	if (!setup_redirections(cmd))
	{
		perror("redirection failed");
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		free(cmd_path);
		exit(1);
	}
	if (!execve(cmd_path, cmd->args, shell->envp))
	{
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		free(cmd_path);
		handle_command_errors(cmd_path, cmd->args[0]);
		exit(EXIT_FAILURE);
	}
}
