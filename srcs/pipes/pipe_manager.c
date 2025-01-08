/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:43:39 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/08 21:54:30 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_pipe_io(int in_fd, int out_fd)
{
	if (in_fd != -1)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 failed in_fd");
			return (0);
		}
		close(in_fd);
	}
	if (out_fd != -1)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 failed out_fd");
			return (0);
		}
		close(out_fd);
	}
	return (1);
}

void	handle_parent_process(int *prev_pipe, int *pipe_fd)
{
	if (prev_pipe)
		close_pipe_ends(prev_pipe);
	if (pipe_fd)
	{
		if (pipe_fd[0] != -1 && pipe_fd[1] != -1)
		{
			if (prev_pipe)
			{
				prev_pipe[0] = pipe_fd[0];
				prev_pipe[1] = pipe_fd[1];
			}
		}
	}
	else
		close_pipe_ends(pipe_fd);
}

static int	setup_pipeline_io(int input_fd, int output_fd, int *prev_pipe,
		int *pipe_fd)
{
	if (!setup_pipe_io(input_fd, output_fd))
	{
		handle_pipe_io_error(prev_pipe, pipe_fd);
		return (0);
	}
	close_pipe_ends(prev_pipe);
	if (pipe_fd && pipe_fd[0] != -1)
	{
		close(pipe_fd[0]);
		pipe_fd[0] = -1;
	}
	return (1);
}

void	handle_pipeline_child(t_command *cmd, int *prev_pipe, int *pipe_fd,
		t_shell *shell)
{
	int	status;
	int	input_fd;
	int	output_fd;

	input_fd = get_input_fd(prev_pipe);
	output_fd = get_output_fd(pipe_fd);
	if (!setup_pipeline_io(input_fd, output_fd, prev_pipe, pipe_fd))
		exit(1);
	setup_child_signal();
	if (!setup_redirections(cmd))
	{
		close_pipe_ends(pipe_fd);
		exit(1);
	}
	status = execute_single_command(cmd, shell);
	close_pipe_ends(pipe_fd);
	exit(status);
}
