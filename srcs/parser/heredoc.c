/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:31:34 by jadyar            #+#    #+#             */
/*   Updated: 2025/01/12 12:27:48 by dodordev         ###   ########.fr       */
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
		return (0);
	return (1);
}

static int	write_to_heredoc(int fd, char *line)
{
	size_t	len;

	if (!line)
		return (0);
	len = ft_strlen(line);
	if (write(fd, line, len) == -1)
		return (0);
	if (write(fd, "\n", 1) == -1)
		return (0);
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

int	setup_heredoc(t_redirection *redir)
{
	int		heredoc_pipe[2];
	char	*line;
	size_t	len_delimiter;

	if (redir->heredoc_processed)
		return (1);
	if (!create_pipe(heredoc_pipe, NULL))
		return (0);
	len_delimiter = ft_strlen(redir->filename);
	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close_pipe_ends(heredoc_pipe);
			return (cleanup_heredoc(heredoc_pipe, false));
		}
		if (handle_heredoc_line(line, redir->filename, len_delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (!write_to_heredoc(heredoc_pipe[1], line))
		{
			free(line);
			close_pipe_ends(heredoc_pipe);
			return (0);
		}
		free(line);
	}
	redir->heredoc_processed = true;
	return (cleanup_heredoc(heredoc_pipe, true));
}
