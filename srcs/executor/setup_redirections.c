/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:05:00 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/14 20:08:58 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_input_file(const char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	if (fd == -1)
	{
		if (errno == ENOENT)
			print_file_error(infile, "No such file or directory");
		else if (errno == EACCES)
			print_file_error(infile, "Permission denied");
		else
			print_file_error(infile, "Error opening file");
		g_exit_status = 1;
	}
	return (fd);
}

int	open_output_file(const char *outfile, int flags)
{
	int	fd;

	fd = open(outfile, flags, FILE_PERMS);
	if (fd == -1)
	{
		if (errno == ENOENT)
			print_file_error(outfile, "No such file");
		else if (errno == EACCES)
			print_file_error(outfile, "Permission denied");
		else
			print_file_error(outfile, strerror(errno));
		g_exit_status = 1;
	}
	return (fd);
}

int	redirect_fd(int old_fd, int new_fd, const char *filename)
{
	if (dup2(old_fd, new_fd) == -1)
	{
		print_file_error(filename, "Error duplicating file descriptor");
		close(old_fd);
		g_exit_status = 1;
		return (0);
	}
	return (1);
}
