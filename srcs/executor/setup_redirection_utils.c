/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirection_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:20:41 by jadyar            #+#    #+#             */
/*   Updated: 2024/12/06 15:28:34 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	backup_std_fds(int *stdin_backup, int *stdout_backup)
{
	*stdin_backup = dup(STDIN_FILENO);
	*stdout_backup = dup(STDOUT_FILENO);
	if (*stdin_backup == -1 || *stdout_backup == -1)
	{
		g_exit_status = 1;
		return (0);
	}
	return (1);
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
		g_exit_status = 1;
		if (errno == EACCES)
			print_file_error(outfile, "Permission denied");
		else if (errno == ENOENT)
			print_file_error(outfile, "No such file");
		else
			print_file_error(outfile, strerror(errno));
	}
	return (fd);
}
