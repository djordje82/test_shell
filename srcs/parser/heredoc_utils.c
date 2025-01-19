/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:14:49 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/19 20:18:57 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_heredoc_end(int *fd)
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
		return (0);
	return (1);
}

int	process_heredoc_lines(int heredoc_pipe[2], t_redirection *redir,
		size_t len_delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close_heredoc_end(&heredoc_pipe[1]);
			return (1);
		}
		if (handle_heredoc_line(line, redir->filename, len_delimiter) == 0)
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
