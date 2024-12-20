/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirection_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:20:41 by jadyar            #+#    #+#             */
/*   Updated: 2024/12/20 11:35:45 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	backup_std_fds(int *stdin_backup, int *stdout_backup)
{
	*stdin_backup = dup(STDIN_FILENO);
	if (*stdin_backup == -1)
	{
		g_exit_status = 1;
		return (0);
	}
	*stdout_backup = dup(STDOUT_FILENO);
	if (*stdout_backup == -1)
	{
		close(*stdin_backup);
		g_exit_status = 1;
		return (0);
	}
	return (1);
}

static int	handle_regular_input(t_command *cmd)
{
	int	fd;

	fd = open(cmd->infile, O_RDONLY);
	if (fd == -1)
	{
		g_exit_status = 1;
		handle_file_open_error(cmd->infile);
		return (0);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		print_file_error(cmd->infile, "Error duplicating file descriptor");
		g_exit_status = 1;
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

static int	handle_input_redirection(t_command *cmd)
{
	static bool	heredoc_processed = false;

	if (cmd->in_type == REDIR_INPUT)
		return (handle_regular_input(cmd));
	else if (cmd->in_type == REDIR_HEREDOC)
	{
		if (heredoc_processed)
			return (1);
		if (!setup_heredoc(cmd))
		{
			g_exit_status = 1;
			return (0);
		}
		heredoc_processed = true;
		return (1);
	}
	return (1);
}

static int	handle_output_redirection(t_command *cmd)
{
	int	fd;
	int	flags;

	if (!cmd->outfile || !cmd->out_type)
		return (1);
	flags = O_WRONLY | O_CREAT;
	if (cmd->out_type == REDIR_TRUNC)
		flags |= O_TRUNC;
	else if (cmd->out_type == REDIR_APPEND)
		flags |= O_APPEND;
	fd = open_output_file(cmd->outfile, flags);
	if (fd == -1)
		return (0);
	if (!redirect_output(fd, cmd->outfile))
		return (0);
	close(fd);
	return (1);
}

int	setup_redirections(t_command *cmd)
{
	int	stdin_backup;
	int	stdout_backup;

	stdin_backup = -1;
	stdout_backup = -1;
	if (!backup_std_fds(&stdin_backup, &stdout_backup) || !cmd)
		return (0);
	if (cmd->infile && (!handle_input_redirection(cmd)))
	{
		g_exit_status = 1;
		restore_std_fds(STDIN_FILENO, STDOUT_FILENO);
		close(stdin_backup);
		close(stdout_backup);
		return (0);
	}
	if (cmd->outfile && (!handle_output_redirection(cmd)))
	{
		g_exit_status = 1;
		restore_std_fds(STDIN_FILENO, STDOUT_FILENO);
		close(stdin_backup);
		close(stdout_backup);
		return (0);
	}
	return (1);
}
