/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:43:39 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/09 12:36:41 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_pipe(int pipe_fd[2], t_shell *shell)
{
	if (!pipe_fd)
		return (0);
	if (pipe(pipe_fd) == -1)
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
		if (shell)
			perror("pipe failed");
		return (0);
	}
	return (1);
}

void	close_pipe_ends(int pipe_fd[2])
{
	if (!pipe_fd)
		return ;
	if (pipe_fd[0] != -1)
	{
		close(pipe_fd[0]);
		pipe_fd[0] = -1;
	}
	if (pipe_fd[1] != -1)
	{
		close(pipe_fd[1]);
		pipe_fd[1] = -1;
	}
}

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
			//close(out_fd);
			return (0);
		}
		close(out_fd);
	}
	return (1);
}

void	handle_parent_process(int *prev_pipe, int *pipe_fd)
{
	close_pipe_ends(prev_pipe);
	if (pipe_fd && pipe_fd[1] != -1 && pipe_fd[0] != -1)
	{
		prev_pipe[0] = pipe_fd[0];
		prev_pipe[1] = pipe_fd[1];
	}
	else
	{
		close_pipe_ends(pipe_fd);
	}
}

void	handle_pipeline_child(t_command *cmd, int *prev_pipe, int *pipe_fd,
		t_shell *shell)
{
	int	status;
	int	input_fd;
	int	output_fd;

	input_fd = get_input_fd(prev_pipe);
	output_fd = get_output_fd(pipe_fd);
	if (!setup_pipe_io(input_fd, output_fd))
	{
		handle_pipe_io_error(prev_pipe, pipe_fd);
	}
	close_pipe_ends(prev_pipe);
	if (pipe_fd)
		close(pipe_fd[0]);
	if (!setup_redirections(cmd))
		exit(1);
	setup_child_signal();
	status = execute_single_command(cmd, shell);
	exit(status);
}
