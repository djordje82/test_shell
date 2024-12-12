/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:05:00 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/06 15:29:36 by jadyar           ###   ########.fr       */
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

static int handle_input_redirection(t_command *cmd)
{
    static bool heredoc_processed = false;

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
	int		fd;
	int		flags;
	int		last_error;

	if (!cmd->outfile || !cmd->out_type)
		return (1);
	if (cmd->curr_fd > 2)
	{
		close(cmd->curr_fd);
		cmd->curr_fd = -1;
	}
	flags = O_WRONLY | O_CREAT;
	last_error = 0;
	if (cmd->out_type == REDIR_TRUNC)
		flags |= O_TRUNC;
	else if (cmd->out_type == REDIR_APPEND)
		flags |= O_APPEND;
	fd = open_output_file(cmd->outfile, flags);
	if (fd == -1)
	{
		last_error = errno;
		if (errno == EACCES)
			g_exit_status = 1;
		else if (errno == ENOENT)
			g_exit_status = 1;
		return (0);
	}
	cmd->curr_fd = fd;
	if (!redirect_output(fd, cmd->outfile))
	{
		close(fd);
		if (last_error)
			errno = last_error;
		cmd->curr_fd = -1;
		return (0);
	}
	//close (fd);
	return (1);
}

static void	close_cmd_fd(t_command *cmd)
{
	if (cmd && cmd->curr_fd > 2)
	{
		close(cmd->curr_fd);
		cmd->curr_fd = -1;
	}
}

static int	check_redirection_permissions(t_command *cmd)
{
	struct stat	path_stat;
	char		*parent_dir;
	char		*last_slash;

	if (cmd->outfile)
	{
		if (access(cmd->outfile, F_OK) == 0)
		{
			if (stat(cmd->outfile, &path_stat) == 0)
			{
				if (S_ISDIR(path_stat.st_mode))
				{
					print_file_error(cmd->outfile, "Is a directory");
					g_exit_status = 1;
					return (0);
				}
			}
			if (access(cmd->outfile, W_OK) == -1)
			{
				print_file_error(cmd->outfile, "Permission denied");
				g_exit_status = 1;
				return (0);
			}
		}

		else if (cmd->out_type == REDIR_TRUNC || cmd->out_type == REDIR_APPEND)
		{
			parent_dir = ft_strdup(cmd->outfile);
			last_slash = ft_strrchr(parent_dir, '/');
			if (last_slash)
			{
				*last_slash = '\0';
				if (access(parent_dir, W_OK) == -1)
				{
					print_file_error(parent_dir, "Permission denied");
					g_exit_status = 1;
					free(parent_dir);
					return (0);
				}
			}
			free(parent_dir);
		}
	}
	return (1);
}

int	setup_redirections(t_command *cmd)
{
	int	stdin_backup;
	int	stdout_backup;

	if (!backup_std_fds(&stdin_backup, &stdout_backup) || !cmd)
		return (0);
	if (!check_redirection_permissions(cmd))
	{
		restore_std_fds(stdin_backup, stdout_backup);
		return (0);
	}
	close_cmd_fd(cmd);
	if (cmd->infile && (!handle_input_redirection(cmd)))
	{
		g_exit_status = 1;
		restore_std_fds(STDIN_FILENO, STDOUT_FILENO);
		return (0);
	}
	if (cmd->outfile && (!handle_output_redirection(cmd)))
	{
		g_exit_status = 1;
		restore_std_fds(STDIN_FILENO, STDOUT_FILENO);
		return (0);
	}
	close(stdin_backup);
	close(stdout_backup);
	return (1);
}
