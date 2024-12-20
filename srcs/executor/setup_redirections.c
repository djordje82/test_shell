/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:05:00 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/19 18:48:03 by jadyar           ###   ########.fr       */
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
