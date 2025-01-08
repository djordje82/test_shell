/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:31:34 by jadyar            #+#    #+#             */
/*   Updated: 2025/01/08 17:33:42 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_heredoc_line(char *line, const char *delimiter,
		size_t len_delimiter)
{
	size_t	line_len;

	if (!line)
		return (-1);
	line_len = ft_strlen(line);
	if (line_len > 0 && line[line_len - 1] == '\n')
		line[line_len - 1] = '\0';
	if (ft_strlen(line) == len_delimiter && ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (0);
	}
	return (1);
}

static int	write_to_heredoc(int fd, char *line)
{
	size_t	len;

	if (!line)
		return (0);
	len = ft_strlen(line);
	if (write(fd, line, len) == -1 || write(fd, "\n", 1) == -1)
	{
		free(line);
		return (0);
	}
	free(line);
	return (1);
}

static int	cleanup_heredoc(int *heredoc_pipe, bool is_last)
{
	if (heredoc_pipe[1] != -1)
		close(heredoc_pipe[1]);
	if (is_last)
	{
		if (heredoc_pipe[0] != -1 && dup2(heredoc_pipe[0], STDIN_FILENO) == -1)
		{
			if (heredoc_pipe[0] != -1)
				close(heredoc_pipe[0]);
			return (0);
		}
	}
	if (heredoc_pipe[0] != -1)
		close(heredoc_pipe[0]);
	return (1);
}

int setup_heredoc(t_command *cmd)
{
    t_redirection *redir;
    int heredoc_pipe[2];
    char *line;
    size_t len_delimiter;
    bool success;

    // First, find the last heredoc redirection in the list
    // This maintains compatibility with how bash processes multiple heredocs
    redir = cmd->redirections;
    success = true;

    while (redir)
    {
        if (redir->type == TOKEN_HEREDOC)
        {
            // Set up pipe for this heredoc
            if (!create_pipe(heredoc_pipe, NULL))
                return (0);

            len_delimiter = ft_strlen(redir->filename);
            setup_heredoc_signals();

            // Process the heredoc content
            while (1)
            {
                line = readline("> ");
                if (!line)
                    return (cleanup_heredoc(heredoc_pipe, false));

                // Check if we've reached the delimiter
                if (handle_heredoc_line(line, redir->filename, len_delimiter) == 0)
                    break;

                // Write the line to heredoc pipe
                if (!write_to_heredoc(heredoc_pipe[1], line))
                {
                    close_pipe_ends(heredoc_pipe);
                    return (0);
                }
            }

            // Only keep the last heredoc's file descriptor
            if (!cleanup_heredoc(heredoc_pipe, redir->next == NULL))
            {
                success = false;
                break;
            }
        }
        redir = redir->next;
    }

    return (success);
}
