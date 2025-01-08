/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 22:12:20 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/08 22:17:59 by dodordev         ###   ########.fr       */
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

static int	process_heredoc_input(int *heredoc_pipe, const char *delimiter, 
	size_t len_delimiter)
{
	char	*line;

	line = readline("> ");
	if (!line)
		return (cleanup_heredoc(heredoc_pipe, false));
	if (handle_heredoc_line(line, delimiter, len_delimiter) == 0)
		return (0);
	if (!write_to_heredoc(heredoc_pipe[1], line))
	{
		close_pipe_ends(heredoc_pipe);
		return (-1);
	}
	return (1);
}

bool	setup_single_heredoc(t_redirection *redir, int *heredoc_pipe)
{
	size_t	len_delimiter;
	int		read_status;

	if (!create_pipe(heredoc_pipe, NULL))
		return (false);
	len_delimiter = ft_strlen(redir->filename);
	setup_heredoc_signals();
	while (1)
	{
		read_status = process_heredoc_input(heredoc_pipe, redir->filename, 
				len_delimiter);
		if (read_status <= 0)
			break ;
	}
	if (read_status == -1)
		return (false);
	return (cleanup_heredoc(heredoc_pipe, redir->next == NULL));
}
