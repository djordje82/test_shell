/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:14:49 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/15 18:06:28 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_heredoc_end(int	*fd)
{
	if (*fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}

static int	write_to_heredoc(int fd, char *line)
{
	if (fd == -1)
		return (0);
	if (!line)
		return (1);
	if (write(fd, line, ft_strlen(line)) == -1)
		return (0);
	if (write(fd, "\n", 1) == -1)
		return (0);
	return (1);
}

int process_heredoc_lines(int heredoc_pipe[2], t_redirection *redir, size_t len_delimiter)
{
    char *line;

    while (1)
    {
		line = readline("> ");

        if (!line)
        {
            close_heredoc_end(&heredoc_pipe[1]);
            return (1);
        }

        if (ft_strlen(line) == len_delimiter && 
            ft_strncmp(line, redir->filename, len_delimiter) == 0)
        {
            free(line);
            close_heredoc_end(&heredoc_pipe[1]);
            return (1);
        }

        if (!write_to_heredoc(heredoc_pipe[1], line))
        {
            free(line);
            close_heredoc_end(&heredoc_pipe[1]);
            return (0);
        }
        free(line);
    }
}
