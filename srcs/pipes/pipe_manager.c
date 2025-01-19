/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:43:39 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/19 16:21:40 by dodordev         ###   ########.fr       */
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
	{
		if (prev_pipe[0] != -1)
			close(prev_pipe[0]);
		if (prev_pipe[1] != -1)
			close(prev_pipe[1]);
		prev_pipe[0] = -1;
		prev_pipe[1] = -1;
	}
	if (pipe_fd && prev_pipe)
	{
		prev_pipe[0] = pipe_fd[0];
		prev_pipe[1] = pipe_fd[1];
	}
}

static int	setup_pipeline_io(int input_fd, int output_fd, int *prev_pipe,
		int *pipe_fd)
{
	if (!setup_pipe_io(input_fd, output_fd))
	{
		handle_pipe_io_error(prev_pipe, pipe_fd);
		return (0);
	}
	if (input_fd != -1)
		close(input_fd);
	if (output_fd != -1)
		close(output_fd);
	if (prev_pipe)
	{
		close_pipe_ends(prev_pipe);
	}
	if (pipe_fd)
	{
		if (pipe_fd[0] != -1 && pipe_fd[0] != input_fd)
			close(pipe_fd[0]);
		if (pipe_fd[1] != -1 && pipe_fd[1] != output_fd)
			close(pipe_fd[1]);
	}
	return (1);
}

void	handle_pipeline_child(t_command *cmd, int *prev_pipe, int *pipe_fd,
		t_shell *shell)
{
	int	status;
	int	input_fd;
	int	output_fd;

	if (cmd->redirections && cmd->redirections->type == TOKEN_HEREDOC)
        input_fd = cmd->redirections->heredoc_fd;
    else
        input_fd = get_input_fd(prev_pipe);

    output_fd = get_output_fd(pipe_fd);

    if (!setup_pipeline_io(input_fd, output_fd, prev_pipe, pipe_fd))
    {
        handle_pipe_io_error(prev_pipe, pipe_fd);
        exit(1);
    }
	setup_child_signal();
	status = execute_single_command(cmd, shell);
	close_pipe_ends(pipe_fd);
	exit(status);
}
