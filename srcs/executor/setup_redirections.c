/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:05:00 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/03 15:12:42 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function handles regular input redirection. It opens the input file with the appropriate flags and duplicates the file descriptor to stdin.*/
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

/*This function handles input redirection. It opens the
input file with the appropriate flags and duplicates
the file descriptor to stdin.*/
static int	handle_input_redirection(t_command *cmd)
{
	if (cmd->in_type == REDIR_INPUT)
		return (handle_regular_input(cmd));
	else if (cmd->in_type == REDIR_HEREDOC)
		return (setup_heredoc(cmd));
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
	fd = open(cmd->outfile, flags, FILE_PERMS);
	if (fd == -1)
	{
		g_exit_status = 1;
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

int	setup_redirections(t_command *cmd)
{
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
	return (1);
}
