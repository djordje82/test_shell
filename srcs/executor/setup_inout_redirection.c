/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_inout_redirection.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 22:33:50 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/08 22:48:45 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_input_file(const char *filename, struct stat *st)
{
	if (stat(filename, st) == -1)
	{
		if (errno == ENOENT)
			print_file_error(filename, "No such file or directory");
		else
			print_file_error(filename, strerror(errno));
		g_exit_status = 1;
		return (0);
	}
	if (!S_ISREG(st->st_mode))
	{
		print_file_error(filename, "Not a regular file");
		return (0);
	}
	if (access(filename, R_OK) == -1)
	{
		print_file_error(filename, "Permission denied");
		return (0);
	}
	return (1);
}

int	handle_input_redirection(t_redirection *redir)
{
	int			fd;
	struct stat	st;

	if (!validate_input_file(redir->filename, &st))
		return (0);
	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		print_file_error(redir->filename, strerror(errno));
		return (0);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

static int	setup_output_flags(t_redirection *redir, int *flags)
{
	*flags = O_WRONLY | O_CREAT;
	if (redir->type == TOKEN_REDIR_OUT)
		*flags |= O_TRUNC;
	else if (redir->type == TOKEN_APPEND)
		*flags |= O_APPEND;
	return (1);
}

static int	process_single_output(t_redirection *redir, int *fd)
{
	int	flags;

	if (*fd != -1)
		close(*fd);
	setup_output_flags(redir, &flags);
	*fd = open_output_file(redir->filename, flags);
	if (*fd == -1)
		return (0);
	if (redir->next == NULL || (redir->next
			&& redir->next->type != TOKEN_REDIR_OUT
			&& redir->next->type != TOKEN_APPEND))
	{
		if (!redirect_output(*fd, redir->filename))
		{
			close(*fd);
			return (0);
		}
	}
	return (1);
}

int	handle_output_redirection(t_command *cmd)
{
	t_redirection	*redir;
	int				fd;
	int				success;

	success = 1;
	fd = -1;
	redir = cmd->redirections;
	while (redir && success)
	{
		if (redir->type == TOKEN_REDIR_OUT || redir->type == TOKEN_APPEND)
		{
			if (!process_single_output(redir, &fd))
			{
				success = 0;
				break ;
			}
		}
		redir = redir->next;
	}
	if (fd != -1)
		close(fd);
	return (success);
}
