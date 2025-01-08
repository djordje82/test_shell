/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirection_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 15:20:41 by jadyar            #+#    #+#             */
/*   Updated: 2025/01/08 17:44:17 by dodordev         ###   ########.fr       */
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

/* static int handle_regular_input(t_command *cmd)
{
    t_redirection *redir;
    int fd;
    int success;

    // Initialize success flag
    success = 1;
    redir = cmd->redirections;

    // Process each input redirection in sequence
    while (redir && success)
    {
        // We only want to process regular input redirections here, not heredocs
        if (redir->type == TOKEN_REDIR_IN)
        {
            // If we had a previous file descriptor open, close it
            if (fd != -1)
                close(fd);

            // Try to open the input file
            fd = open(redir->filename, O_RDONLY);
            if (fd == -1)
            {
                g_exit_status = 1;
                handle_file_open_error(redir->filename);
                success = 0;
                break;
            }

            // Only redirect to STDIN if this is the final input redirection
            // This matches bash behavior where only the last redirection takes effect
            if (redir->next == NULL || 
                (redir->next && redir->next->type != TOKEN_REDIR_IN))
            {
                if (dup2(fd, STDIN_FILENO) == -1)
                {
                    print_file_error(redir->filename, 
                        "Error duplicating file descriptor");
                    g_exit_status = 1;
                    close(fd);
                    success = 0;
                    break;
                }
            }
        }
        redir = redir->next;
    }

    // Clean up our file descriptor
    if (fd != -1)
        close(fd);

    return success;
} */

static int handle_input_redirection(t_redirection *redir)
{
    int fd;
    struct stat st;

    if (stat(redir->filename, &st) == -1) {
        if (errno == ENOENT)
            print_file_error(redir->filename, "No such file or directory");
        else
            print_file_error(redir->filename, strerror(errno));
        return (0);
    }

    if (!S_ISREG(st.st_mode)) {
        print_file_error(redir->filename, "Not a regular file");
        return (0);
    }

    if (access(redir->filename, R_OK) == -1) {
        print_file_error(redir->filename, "Permission denied");
        return (0);
    }

    fd = open(redir->filename, O_RDONLY);
    if (fd == -1) {
        print_file_error(redir->filename, strerror(errno));
        return (0);
    }

    if (dup2(fd, STDIN_FILENO) == -1) {
        close(fd);
        return (0);
    }
    close(fd);
    return (1);
}

static int handle_output_redirection(t_command *cmd)
{
    t_redirection *redir;
    int fd;
    int flags;
    int success;

    // Initialize our success flag
    success = 1;
    redir = cmd->redirections;

    // Iterate through all redirections
    while (redir && success)
    {
        // Only process output redirections
        if (redir->type == TOKEN_REDIR_OUT || redir->type == TOKEN_APPEND)
        {
            // Close previous file descriptor if we had one
            if (fd != -1)
                close(fd);

            // Set up the appropriate flags based on redirection type
            flags = O_WRONLY | O_CREAT;
            if (redir->type == TOKEN_REDIR_OUT)  // equivalent to old REDIR_TRUNC
                flags |= O_TRUNC;
            else if (redir->type == TOKEN_APPEND) // equivalent to old REDIR_APPEND
                flags |= O_APPEND;

            // Try to open the output file
            fd = open_output_file(redir->filename, flags);
            if (fd == -1)
            {
                success = 0;
                break;
            }

            // Only redirect to STDOUT if this is the final output redirection
            // This matches bash behavior where only the last redirection takes effect
            if (redir->next == NULL || 
                (redir->next && redir->next->type != TOKEN_REDIR_OUT && 
                 redir->next->type != TOKEN_APPEND))
            {
                if (!redirect_output(fd, redir->filename))
                {
                    close(fd);
                    success = 0;
                    break;
                }
            }
        }
        redir = redir->next;
    }

    // Clean up our file descriptor
    if (fd != -1)
        close(fd);

    return success;
}

int setup_redirections(t_command *cmd)
{
    t_redirection *redir;
    int stdin_backup;
    int stdout_backup;

    if (!backup_std_fds(&stdin_backup, &stdout_backup))
        return (0);

    redir = cmd->redirections;
    while (redir)
    {
        if (redir->type == TOKEN_REDIR_IN || redir->type == TOKEN_HEREDOC)
        {
            if (!handle_input_redirection(redir))
            {
                restore_std_fds(stdin_backup, stdout_backup);
                return (0);
            }
        }
        else if (redir->type == TOKEN_REDIR_OUT || redir->type == TOKEN_APPEND)
        {
            if (!handle_output_redirection(cmd))
            {
                restore_std_fds(stdin_backup, stdout_backup);
                return (0);
            }
        }
        redir = redir->next;
    }

    return (1);
}