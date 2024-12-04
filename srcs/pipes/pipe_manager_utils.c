/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_manager_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 18:19:41 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 18:23:43 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_input_fd(int *prev_pipe)
{
	if (prev_pipe)
		return (prev_pipe[0]);
	return (-1);
}

int	get_output_fd(int *pipe_fd)
{
	if (pipe_fd)
		return (pipe_fd[1]);
	return (-1);
}

void	handle_pipe_io_error(int *prev_pipe, int *pipe_fd)
{
	perror("pipe io failed");
	close_pipe_ends(prev_pipe);
	if (pipe_fd)
		close_pipe_ends(pipe_fd);
	exit(1);
}
