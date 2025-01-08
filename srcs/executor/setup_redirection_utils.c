/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirection_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:20:41 by jadyar            #+#    #+#             */
/*   Updated: 2025/01/08 22:24:43 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_file_open_error(const char *file)
{
	if (errno == ENOENT)
		print_file_error(file, "No such file or directory");
	else if (errno == EACCES)
		print_file_error(file, "Permission denied");
	else
		print_file_error(file, strerror(errno));
}

int	redirect_output(int fd, char *outfile)
{
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		print_command_error(outfile, "Error duplicating file descriptor");
		close(fd);
		g_exit_status = 1;
		return (0);
	}
	return (1);
}

int	open_output_file(char *outfile, int flags)
{
	int	fd;

	fd = open(outfile, flags, FILE_PERMS);
	if (fd == -1)
	{
		if (errno == EACCES)
			print_file_error(outfile, "Permission denied");
		else if (errno == ENOENT)
			print_file_error(outfile, "No such file");
		else
			print_file_error(outfile, strerror(errno));
		g_exit_status = 1;
	}
	return (fd);
}

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
