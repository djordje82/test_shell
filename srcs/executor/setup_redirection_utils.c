/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirection_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:20:41 by jadyar            #+#    #+#             */
/*   Updated: 2024/12/15 13:16:42 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_input_redirection(t_command *cmd)
{
	static bool	heredoc_processed;

	if (cmd->in_type == REDIR_INPUT)
		return (handle_regular_input(cmd->infile));
	else if (cmd->in_type == REDIR_HEREDOC && !heredoc_processed)
	{
		if (!setup_heredoc(cmd))
		{
			g_exit_status = 1;
			return (0);
		}
		heredoc_processed = true;
	}
	return (1);
}

static int	handle_output_redirection(t_command *cmd)
{
	int	flags;
	int	fd;

	flags = O_WRONLY | O_CREAT;
	if (cmd->out_type == REDIR_TRUNC)
		flags |= O_TRUNC;
	else
		flags |= O_APPEND;
	fd = open_output_file(cmd->outfile, flags);
	if (fd == -1)
		return (0);
	if (!redirect_fd(fd, STDOUT_FILENO, cmd->outfile))
	{
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

int	handle_regular_input(const char *infile)
{
	int	fd;

	fd = open_input_file(infile);
	if (fd == -1)
		return (0);
	if (!redirect_fd(fd, STDIN_FILENO, infile))
		return (0);
	close(fd);
	return (1);
}

int	setup_redirections(t_command *cmd)
{
	int	stdin_backup;
	int	stdout_backup;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (stdin_backup == -1 || stdout_backup == -1)
	{
		g_exit_status = 1;
		return (0);
	}
	if (cmd->infile && !handle_input_redirection(cmd))
	{
		restore_std_fds(stdin_backup, stdout_backup);
		return (0);
	}
	if (cmd->outfile && !handle_output_redirection(cmd))
	{
		restore_std_fds(stdin_backup, stdout_backup);
		return (0);
	}
	close(stdin_backup);
	close(stdout_backup);
	return (1);
}

void	restore_std_fds(int stdin_backup, int stdout_backup)
{
	if (dup2(stdin_backup, STDIN_FILENO) == -1 
		|| dup2(stdout_backup, STDOUT_FILENO) == -1)
		exit(1);
	close(stdin_backup);
	close(stdout_backup);
}
