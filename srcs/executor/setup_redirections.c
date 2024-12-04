/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:05:00 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 17:20:34 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_regular_input(t_command *cmd)
{
	int	fd;

	fd = open(cmd->infile, O_RDONLY);
	if (fd == -1)
	{
		g_exit_status = 1;
		if (errno == ENOENT)
			print_file_error(cmd->infile, "No such file or directory");
		else if (errno == EACCES)
			print_file_error(cmd->infile, "Permission denied");
		else
			print_file_error(cmd->infile, "Error opening file");
		return (0);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		print_file_error(cmd->infile, "Error duplicating file descriptor");
		close(fd);
		g_exit_status = 1;
		return (0);
	}
	close(fd);
	return (1);
}

static int	handle_input_redirection(t_command *cmd)
{
	if (cmd->in_type == REDIR_INPUT)
		return (handle_regular_input(cmd));
	else if (cmd->in_type == REDIR_HEREDOC)
		return (setup_heredoc(cmd));
	return (1);
}

//TO DO: SPLIT
static int	handle_output_redirection(t_command *cmd)
{
	int		fd;
	int		flags;

	if (!cmd->outfile || !cmd->out_type)
		return (1);
	flags = O_WRONLY | O_CREAT;
	if (cmd->out_type == REDIR_TRUNC)
		flags |= O_TRUNC;
	else if (cmd->out_type == REDIR_APPEND)
		flags |= O_APPEND;
	fd = open(cmd->outfile, flags, FILE_PERMS);
	if (fd == -1)
	{
		g_exit_status = 1;
		if (errno == EACCES)
			print_file_error(cmd->outfile, "Permission denied");
		else if (errno == ENOENT)
			print_file_error(cmd->outfile, "No fucking file!!");
		else
			print_file_error(cmd->outfile, strerror(errno));
		return (0);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		print_command_error(cmd->outfile, "Error duplicating file descriptor");
		close(fd);
		g_exit_status = 1;
		return (0);
	}
	close(fd);
	return (1);
}

//TO DO: SPLIT
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
	if (!cmd)
		return (0);
	if (cmd->infile)
	{
		if (!handle_input_redirection(cmd))
		{
			g_exit_status = 1;
			restore_std_fds(STDIN_FILENO, STDOUT_FILENO);
			return (0);
		}
	}
	if (cmd->outfile)
	{
		if (!handle_output_redirection(cmd))
		{
			g_exit_status = 1;
			restore_std_fds(STDIN_FILENO, STDOUT_FILENO);
			return (0);
		}
	}
	close(stdin_backup);
	close(stdout_backup);
	return (1);
}
